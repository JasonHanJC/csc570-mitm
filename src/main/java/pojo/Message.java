package pojo;

import java.io.Serializable;
import java.net.InetSocketAddress;

public class Message implements Serializable {
    public Message(Header header, String body) {
        this.header = header;
        this.body = body;
    }

    public static class Header implements Serializable {
        public Header(InetSocketAddress clientAddress, InetSocketAddress serverAddress) {
            this.clientAddress = clientAddress;
            this.serverAddress = serverAddress;
        }

        public InetSocketAddress getClientAddress() {
            return clientAddress;
        }

        public void setClientAddress(InetSocketAddress clientAddress) {
            this.clientAddress = clientAddress;
        }

        public InetSocketAddress getServerAddress() {
            return serverAddress;
        }

        public void setServerAddress(InetSocketAddress serverAddress) {
            this.serverAddress = serverAddress;
        }

        private InetSocketAddress clientAddress;
        private InetSocketAddress serverAddress;
    }

    public Header getHeader() {
        return header;
    }

    public void setHeader(Header header) {
        this.header = header;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    private Header header;
    private String body;
}