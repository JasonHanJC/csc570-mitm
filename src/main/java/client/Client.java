package client;

import java.io.FileInputStream;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class Client extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        // Load the FXML
        VBox root = (VBox) FXMLLoader.load(getClass().getResource("/fxml/client/client.fxml"));

        // Create the Scene
        Scene scene = new Scene(root);
        // Set the Scene to the Stage
        primaryStage.setScene(scene);
        // Set the Title to the Stage
        primaryStage.setTitle("Client GUI");
        // Display the Stage
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
