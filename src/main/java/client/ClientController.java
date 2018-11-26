package client;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import pojo.Message;

public class ClientController {
    private static final String HIJACK_ADDRESS = "127.0.0.1";
    private static final String PROXY_ADDRESS = "127.0.0.1";
    private static final int PROXY_PORT = 50005;

    @FXML
    private TextField textField;

    @FXML
    private Button sendButton;

    @FXML
    private Label label;

    public ClientController() { }

    @FXML
    private void initialize() {
        sendButton.setOnAction((actionEvent)->{
            sendMessage();
        });
        textField.setOnAction((actionEvent)->{
            sendMessage();
        });
    }

    private void sendMessage() {
        String targetAddress = "127.0.0.1";
        int targetPort = 50000;
        String connectAddress = targetAddress.equals(HIJACK_ADDRESS) ? PROXY_ADDRESS : targetAddress;
        int connectPort = targetAddress.equals(HIJACK_ADDRESS) ? PROXY_PORT : targetPort;

        try {
            Message.Header header = new Message.Header(
                    new InetSocketAddress(InetAddress.getLocalHost().getHostAddress(), connectPort),
                    new InetSocketAddress(targetAddress, targetPort));
            Message message = new Message(header, textField.getText());
            Socket socket = new Socket(connectAddress, connectPort);
            ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
            oos.writeObject(message);

            label.setText("Sending message: " + message + "...\n");
            ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
            Message returnMessage = (Message) ois.readObject();

            label.setText(label.getText()+ "Returned from server: " + returnMessage);
            oos.flush();
            oos.close();
            ois.close();
        } catch (Exception e) {
            e.printStackTrace();
            label.setText(label.getText()+"\n"+e.toString());
        }

    }
}
