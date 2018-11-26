package proxy;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

import pojo.Message;

enum Decision {
    NOMODIFY,
    IGNORE,
    MODIFY
}

public class Proxy {
    private static final int LISTENING_PORT = 50005;

    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(LISTENING_PORT);
            System.out.println("Proxy listening on port " + LISTENING_PORT);
            while (true) {
                Socket socketFromClient = serverSocket.accept();
                ObjectInputStream sfcois = new ObjectInputStream(socketFromClient.getInputStream());
                Message messageFromClient = (Message) sfcois.readObject();

                System.out.println("Received from client: " + messageFromClient);

                InetSocketAddress serverAddress = messageFromClient.getHeader().getServerAddress();
                InetSocketAddress clientAddress = messageFromClient.getHeader().getClientAddress();

                Decision decisionToServer = decide();
                System.out.println("Let's " + decisionToServer.name() + " this message");
                if (decisionToServer == Decision.IGNORE) {
                    continue;
                }

                String messageToServerBody = decisionToServer == Decision.MODIFY ? new StringBuilder(messageFromClient.getBody()).reverse().toString() : messageFromClient.getBody();
                Message messageToServer = new Message(
                        new Message.Header(
                                clientAddress,
                                serverAddress
                        ),
                        messageToServerBody
                );

                System.out.println("Sending " + messageToServer);

                Socket socketToServer = new Socket(serverAddress.getHostName(), serverAddress.getPort());
                ObjectOutputStream stsoos = new ObjectOutputStream(socketToServer.getOutputStream());
                stsoos.writeObject(messageToServer);
                ObjectInputStream stsois = new ObjectInputStream(socketToServer.getInputStream());
                Message messageFromServer = (Message) stsois.readObject();
                stsoos.flush();
                stsoos.close();
                stsois.close();
                socketToServer.close();

                System.out.println("Received from server: \n"+messageFromServer);

                Decision decisionToClient = decide();
                System.out.println("Let's " + decisionToClient.name() + " this message");
                if (decisionToClient == Decision.IGNORE) {
                    continue;
                }

                String messageToClientBody = decisionToClient == Decision.MODIFY ? new StringBuilder(messageFromServer.getBody()).reverse().toString() : messageFromServer.getBody();
                Message messageToClient = new Message(
                        new Message.Header(
                                messageFromClient.getHeader().getServerAddress(),
                                messageFromClient.getHeader().getClientAddress()
                        ),
                        messageToClientBody
                );
                System.out.println("Sending " + messageToClient);

                ObjectOutputStream sfcoos = new ObjectOutputStream(socketFromClient.getOutputStream());
                sfcoos.writeObject(messageToClient);

                sfcoos.flush();
                sfcoos.close();
                socketFromClient.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (serverSocket != null) {
                try {
                    serverSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static Decision decide(){
        Random random = new Random();
        switch (random.nextInt(3)) {
            case 0: return Decision.NOMODIFY;
            case 1: return Decision.IGNORE;
            default: return Decision.MODIFY;
        }
    }
}
