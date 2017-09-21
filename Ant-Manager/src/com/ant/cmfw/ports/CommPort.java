package com.ant.cmfw.ports;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

abstract public class CommPort {
    private static final String TAG = "CommPort";
    protected String mPortName;

    private byte mPresentHeaderId = 0;

    private ListeningThread mListeningThread;
    private boolean mIsListeningThreadOn;
    private CommPortListener mListener;

    private String mDownloadFilePath;

    public CommPort(String portName) {
        this.mPortName = portName;
    }

    abstract public boolean isOpened();

    abstract public void close();

    abstract protected OutputStream getOutputStream();

    abstract protected InputStream getInputStream();

    public void runListeningThread(CommPortListener listener, String downloadFilePath) {
        this.mListener = listener;
        this.mDownloadFilePath = downloadFilePath;

        this.mIsListeningThreadOn = true;

        this.mListeningThread = new ListeningThread();
        this.mListeningThread.start();
    }

    public void stopListeningThread() {
        this.mIsListeningThreadOn = false;
    }

    class ListeningThread extends Thread {
        @Override
        public void run() {
            listenRawMessage();
        }
    }

    private void suddenlyClose(String reasonMessage) {
        if (this.isOpened()) {
            Log.e(TAG, "[" + this.mPortName + "] Suddenly closed: " + reasonMessage);
            this.close();

            this.mListener.onSuddenlyClosed(this);
        } else {
        }
    }

    public void listenRawMessage() {
        InputStream inputStream = this.getInputStream();
        if (inputStream == null) return;
        DataInputStream dataInputStream = new DataInputStream(inputStream);

        final int kMessageMetadata = 1;
        final int kMessageData = 2;
        final int kFileMetadata = 3;
        final int kFileData = 4;

        boolean isMessageCompleted = false;
        int expectedPayloadType = kMessageMetadata;
        CommPayloadMessageMetadata messageMetadata = null;
        CommPayloadFileMetadata fileMetadata = null;
        byte[] totalMessageData = null;
        BufferedOutputStream bufferedFileOutputStream = null;
        int loadedBytesSize = 0;
        File fileToWrite = null;

        while (this.mIsListeningThreadOn) {
            try {
                // Read header
                CommRawPacketHeader header = CommRawPacketHeader.read(dataInputStream);
                if (header == null) {
                    suddenlyClose("listenRawMessage: fail to get header");
                    return;
                }

                // Read payload
                switch (expectedPayloadType) {
                    case kMessageMetadata:
                        // Message metadata is expected
                        Log.d(TAG, "listenRawMessage(): start to listen");
                        if (!header.getFlagIsMetadata()) {
                            suddenlyClose("listenRawMessage: unexpected payload type: " + header
                                    .getHeaderFlag() + " / expected message metadata");
                            return;
                        }
                        messageMetadata = CommPayloadMessageMetadata.read(dataInputStream);

                        totalMessageData = new byte[messageMetadata.getMessageDataLength()];
                        expectedPayloadType = kMessageData;
                        break;

                    case kMessageData:
                        // Message data is expected
                        if (!header.getFlagIsData()) {
                            suddenlyClose("listenRawMessage: unexpected payload type: " + header
                                    .getHeaderFlag() + " / expected message data");
                            return;
                        }
                        CommPayloadData messageData = CommPayloadData.read(dataInputStream,
                                header.getPayloadSize());
                        byte[] messageDataBytes = messageData.toByteArray();
                        if (messageDataBytes == null) {
                            this.suddenlyClose("listenRawMessage: Failed to make message data " +
                                    "packet");
                            return;
                        }
                        if (totalMessageData == null) {
                            this.suddenlyClose("listenRawMessage: totalMessageData is null");
                            return;
                        }
                        System.arraycopy(messageDataBytes, 0, totalMessageData, loadedBytesSize,
                                messageData.getBytesSize());
                        loadedBytesSize += messageData.getBytesSize();
                        if (header.getFlagIsEndData()) {
                            if (messageMetadata.getIsFileAttached() != 0) {
                                expectedPayloadType = kFileMetadata;
                            } else {
                                // Message without file completed
                                expectedPayloadType = kMessageMetadata;
                                isMessageCompleted = true;
                            }
                        } else {
                            expectedPayloadType = kMessageData;
                        }
                        break;

                    case kFileMetadata:
                        // File metadata is expected
                        if (!header.getFlagIsMetadata()) {
                            suddenlyClose("listenRawMessage: unexpected payload type: " + header
                                    .getHeaderFlag() + " / expected file metadata");
                            return;
                        }
                        fileMetadata = CommPayloadFileMetadata.read(dataInputStream);

                        // Open file
                        String fileName = fileMetadata.getSrcFileName();
                        fileToWrite = new File(this.mDownloadFilePath, fileName);
                        try {
                            bufferedFileOutputStream = new BufferedOutputStream(new
                                    FileOutputStream(fileToWrite));
                        } catch (IOException e) {
                            this.suddenlyClose("listenRawMessage: Failed to get stream for " +
                                    "downloading attached file: " + e.getMessage());
                            return;
                        }

                        // Expect next packet
                        expectedPayloadType = kFileData;
                        break;

                    case kFileData:
                        // File data is expected
                        if (!header.getFlagIsFile()) {
                            suddenlyClose("listenRawMessage: unexpected payload type: " + header
                                    .getHeaderFlag() + " / expected file data");
                            return;
                        }
                        CommPayloadData fileData = CommPayloadData.read(dataInputStream, header
                                .getPayloadSize());

                        // Write
                        byte[] fileDataBytes = fileData.toByteArray();
                        if (fileDataBytes == null) {
                            this.suddenlyClose("listenRawMessage: Failed to make file data packet");
                        }
                        bufferedFileOutputStream.write(fileDataBytes, 0, fileData.getBytesSize());

                        // Expect next packet
                        if (header.getFlagIsEndData()) {
                            // Message with file completed
                            expectedPayloadType = kMessageMetadata;
                            isMessageCompleted = true;
                        } else {
                            expectedPayloadType = kFileData;
                        }
                        break;
                }
            } catch (IOException e) {
                this.suddenlyClose("listenRawMessage: I/O exception (" + e.getMessage() + ")");
                // e.printStackTrace();
                return;
            }

            if (isMessageCompleted) {
                Log.d(TAG, "listenRawMessage(): accept the message");
                // Close the attached file if exists
                if (bufferedFileOutputStream != null) {
                    try {
                        bufferedFileOutputStream.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                // Notify the message
                if (this.mListener != null) {
                    if (fileMetadata != null) {
                        this.mListener.onReceivingRawMessage(totalMessageData, messageMetadata
                                .getMessageDataLength(), fileToWrite.getAbsolutePath());
                        fileToWrite = null;
                    } else {
                        this.mListener.onReceivingRawMessage(totalMessageData, messageMetadata
                                .getMessageDataLength(), null);
                    }
                }

                // Initialize variables
                bufferedFileOutputStream = null;
                expectedPayloadType = kMessageMetadata;
                messageMetadata = null;
                fileMetadata = null;
                totalMessageData = null;
                loadedBytesSize = 0;
                isMessageCompleted = false;
            }
        }
    }

    public int sendRawMessage(byte[] dataBytes, int messageDataLength, File file) {
        if (!this.isOpened()) return -1;

        // Get DataOutputStream
        OutputStream outputStream = this.getOutputStream();
        if (outputStream == null) return -1;
        DataOutputStream dataOutputStream = new DataOutputStream(outputStream);

        Log.d(TAG, "sendRawMessage(byte[]): dataLength=" + messageDataLength);

        // Send message metadata and message data
        {
            // Send message metadata
            this.mPresentHeaderId++;
            CommRawPacket messageMetadataPacket = CommRawPacket.makeMessageMetadataPacket(this
                    .mPresentHeaderId, messageDataLength, (file != null));
            byte[] messageMetadataBytes = messageMetadataPacket.toByteArray();
            if (messageMetadataBytes == null) {
                this.suddenlyClose("sendRawMessage: Failed to make message metadata packet");
                return -1;
            }
            try {
                Log.d(TAG, "metadata packet size: " + messageMetadataPacket.getBytesSize());
                dataOutputStream.write(messageMetadataBytes, 0, messageMetadataPacket
                        .getBytesSize());
            } catch (Exception e) {
                this.suddenlyClose("sendRawMessage: Header write failed");
                return -1;
            }

            // Send message data (split by limited-size packets)
            int sentBytes = 0;
            while (sentBytes < messageDataLength) {
                // Make message data packet
                CommRawPacket messageDataPacket;
                short messageDataPayloadSize;
                boolean isEnd;
                if (CommRawPacketHeader.kMaxPacketPayloadSize < messageDataLength - sentBytes) {
                    // Not end data
                    messageDataPayloadSize = CommRawPacketHeader.kMaxPacketPayloadSize;
                    isEnd = false;
                } else {
                    // end data
                    messageDataPayloadSize = (short) (messageDataLength - sentBytes);
                    isEnd = true;
                }
                byte[] messageDataPayloadBytes = new byte[messageDataPayloadSize];
                System.arraycopy(dataBytes, sentBytes, messageDataPayloadBytes, 0,
                        messageDataPayloadSize);
                messageDataPacket = CommRawPacket.makeDataPacket(this.mPresentHeaderId,
                        messageDataPayloadBytes, sentBytes, messageDataPayloadSize, isEnd, false);
                Log.d(TAG, "[" + this.mPortName + "] sendRawMessage(message): mPayloadSize = " +
                        messageDataPayloadSize);

                // Send
                byte[] messageDataPacketBytes = messageDataPacket.toByteArray();
                if (messageDataPacketBytes == null) {
                    this.suddenlyClose("sendRawMessage: Failed to make message data packet");
                    return -1;
                }
                try {
                    dataOutputStream.write(messageDataPacketBytes, 0, messageDataPacket
                            .getBytesSize());
                } catch (Exception e) {
                    this.suddenlyClose("sendRawMessage: Header write failed");
                    return -1;
                }
                sentBytes += messageDataPayloadSize;
            }
        }

        // If file is attached, send file metadata and file data
        if (file != null) {
            // Send file metadata
            CommRawPacket fileMetadataPacket = CommRawPacket.makeFileMetadataPacket(this
                    .mPresentHeaderId, file);
            byte[] fileMetadataBytes = fileMetadataPacket.toByteArray();
            if (fileMetadataBytes == null) {
                this.suddenlyClose("sendRawMessage: Failed to make file metadata packet");
                return -1;
            }
            try {
                int fileMetadataPacketSize = fileMetadataPacket.getBytesSize();
                dataOutputStream.write(fileMetadataBytes, 0, fileMetadataPacketSize);
            } catch (Exception e) {
                this.suddenlyClose("sendRawMessage: Header write failed");
                return -1;
            }

            // Send file data
            int fileSize = (int) file.length();
            Log.d(TAG, "file size = " + fileSize);
            int sentBytes = 0;
            BufferedInputStream bufferedInputStream = null;
            try {
                bufferedInputStream = new BufferedInputStream(new FileInputStream(file));
            } catch (IOException e) {
                this.suddenlyClose("sendRawMessage: failed to get stream for reading file to be "
                        + "attached");
                return -1;
            }
            while (sentBytes < fileSize) {
                // Make file data packet
                CommRawPacket fileDataPacket;
                short fileDataPayloadSize;
                boolean isEnd;
                if (CommRawPacketHeader.kMaxPacketPayloadSize < fileSize - sentBytes) {
                    // Not end data
                    fileDataPayloadSize = CommRawPacketHeader.kMaxPacketPayloadSize;
                    isEnd = false;
                } else {
                    // end data
                    fileDataPayloadSize = (short) (fileSize - sentBytes);
                    isEnd = true;
                }
                byte[] fileDataPayloadBytes = new byte[fileDataPayloadSize];
                try {
                    int readSize = bufferedInputStream.read(fileDataPayloadBytes, 0,
                            fileDataPayloadSize);
                    if (readSize <= 0) break;
                } catch (IOException e) {
                    if (sentBytes < fileSize) {
                        this.suddenlyClose("sendRawMessage: I/O exception during reading file");
                        return -1;
                    } else break;
                }
                fileDataPacket = CommRawPacket.makeDataPacket(this.mPresentHeaderId,
                        fileDataPayloadBytes, sentBytes, (short) fileDataPayloadSize, isEnd, true);
                Log.d(TAG, "[" + this.mPortName + "] sendRawMessage(file): mPayloadSize = " +
                        fileDataPayloadSize);

                // Send
                byte[] fileDataPacketBytes = fileDataPacket.toByteArray();
                if (fileDataPacketBytes == null) {
                    this.suddenlyClose("sendRawMessage: Failed to make file data packet");
                    return -1;
                }
                try {
                    int fileDataPacketSize = fileDataPacket.getBytesSize();
                    dataOutputStream.write(fileDataPacketBytes, 0, fileDataPacketSize);
                    dataOutputStream.flush();
                } catch (Exception e) {
                    this.suddenlyClose("sendRawMessage: Header write failed");
                    return -1;
                }
                sentBytes += fileDataPayloadSize;
            }
        }
        return 0;
    }
}