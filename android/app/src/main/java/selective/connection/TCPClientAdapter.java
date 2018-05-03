package selective.connection;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

/**
 * Created by eslab on 2017-02-13.
 */

public class TCPClientAdapter extends NetworkAdapter {
    private Socket sock;
    private String ip;
    private int port;

    private String tag = "TCPClientAdapter";

    private BufferedOutputStream writer;
    private BufferedInputStream reader;
    public TCPClientAdapter(short id, String ip, int port) {
        this.ip = ip;
        this.port = port;
        this.dev_id = id;

        writer = null;
        reader = null;

        this.set_controllable();
    }
    @Override
    public boolean device_on() {
        return true;
    }

    @Override
    public boolean device_off() {
        return true;
    }

    @Override
    public boolean make_connection() {
        boolean res = false;
        byte[] bt = new String("What").getBytes();
        send_ctrl_msg(bt, bt.length);

        try {
            if (sock != null)
                sock.close();

            sock = new Socket(ip, port);

            writer = new BufferedOutputStream(sock.getOutputStream());
            reader = new BufferedInputStream(sock.getInputStream());

            res = true;
        } catch (Exception e) {
            Log.d(tag, "Failed to connect to server");

            writer = null;
            reader = null;
            res = false;
        }

        return res;
    }

    @Override
    public boolean close_connection() {
        boolean res = false;
        try {
            sock.close();
            res = true;
            sock = null;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return res;
    }

    @Override
    public int send(byte[] buf, int len) {
        if (writer == null) {
            return -1;
        }

        int res = 0;

        try {
            writer.write(buf, 0, len);
            writer.flush();
            res = len;
        } catch (IOException e) {
            e.printStackTrace();
            res = -1;
        }
        return res;
    }

    @Override
    public int recv(byte[] buf, int len) {
        if (reader == null) {
            Log.d(tag, "reader is null");
            return -1;
        }

        int res = 0;

        try {
            int read_bytes = 0;
            while (read_bytes < len) {
                int bytes = reader.read(buf, read_bytes, len-read_bytes);
                if (bytes < 0) {
                    len = -1;
                    break;
                }

                read_bytes += bytes;
            }
            res = len;
        }catch (Exception e) {
            e.printStackTrace();
            res = -1;
        }
        return res;
    }

    @Override
    public void on_control_recv(byte[] buf, int len) {
        Log.d(tag, new String(Arrays.copyOfRange(buf, 0, len)));
    }
}
