package server;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

import pojo.Message;

public class Server {
    public static void main(String[] args) {
        int listeningPort = 50000;
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(listeningPort);
            System.out.println("server.Server listening on port " + serverSocket.getLocalPort());
            while (true) {
                Socket socket = serverSocket.accept();
                ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
                Message recMessage = (Message) ois.readObject();
                System.out.println("Received message: " + recMessage);

                String targetAddress = recMessage.getHeader().getClientAddress().getHostName();
                Message message = new Message(
                        new Message.Header(
                                new InetSocketAddress(InetAddress.getLocalHost().getHostAddress(), socket.getLocalPort()),
                                new InetSocketAddress(targetAddress, socket.getPort())
                        ),
                        "Received this message: " + recMessage.getBody()
                );
                ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
                oos.writeObject(message);
                oos.flush();
                oos.close();
                ois.close();
                socket.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (serverSocket != null) {
                    serverSocket.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
