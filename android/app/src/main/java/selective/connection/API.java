package selective.connection;

/**
 * Created by eslab on 2017-02-08.
 */

public class API {
    static private API instance = null;
    static String tag = "Comm";
    private API() {
        SegmentManager sm = SegmentManager.get_instance();
        Core nm = Core.get_instance();
    }
    static public API get_instance() {
        if (instance == null)
            instance = new API();
        return instance;
    }

    public int send_data(byte[] buf, int len) {
        int curr_offset = 0;
        int sent_bytes;

        ProtocolData pd = null;
        int packet_size;

        pd = ProtocolManager.data_to_protocol_data(buf, len);
        if (pd == null) throw new AssertionError();

        packet_size = ProtocolManager.serialize(pd, buf, curr_offset, len);
        if (!(packet_size > 0)) throw new AssertionError();
        //Logger.print(tag, "serialized packet size " + packet_size);

        sent_bytes = ProtocolManager.send_packet(packet_size);
        if (sent_bytes < 0) throw new AssertionError();
        //Logger.print(tag, "Sent bytes: " + sent_bytes);

        return sent_bytes;
    }

    public int recv_data(byte[] buf, int len) {
        int ret;
        int packet_size;

        packet_size = ProtocolManager.recv_packet(buf);
        if (buf == null) throw new AssertionError();

        return packet_size;
    }
}