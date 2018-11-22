package client.view

import javafx.scene.control.Button
import javafx.scene.control.Label
import javafx.scene.control.TextField
import javafx.scene.layout.VBox
import pojo.Header
import pojo.Message
import sun.plugin2.liveconnect.ArgumentHelper.writeObject
import tornadofx.*
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.lang.Exception
import java.net.InetAddress
import java.net.InetSocketAddress
import java.net.Socket

const val HIJACK_ADDRESS = "127.0.0.1"
const val PROXY_ADDRESS = "127.0.0.1"
const val PROXY_PORT = 50005

class ClientView: View() {

    override val root:VBox by fxml()
    val textField: TextField by fxid()
    val sendButton: Button by fxid()
    val label: Label by fxid()

    init {
        sendButton.setOnAction {
            sendMessage()
        }
        textField.setOnAction {
            sendMessage()
        }
    }

    private fun sendMessage(){
        val targetAddress = "127.0.0.1"
        val targetPort = 50000
        val connectAddress = if (targetAddress == HIJACK_ADDRESS) PROXY_ADDRESS else targetAddress
        val connectPort = if (targetAddress == HIJACK_ADDRESS) PROXY_PORT else targetPort

        val header = Header(
            InetSocketAddress(InetAddress.getLocalHost().getHostAddress(), connectPort),
            InetSocketAddress(targetAddress, targetPort)
        )
        val message = Message(header, textField.text)
        var socket:Socket? = null
        try {
            socket = Socket(connectAddress, connectPort)
            val oos = ObjectOutputStream(socket.getOutputStream())
            oos.writeObject(message)

            label.text = "Sending message: ${message} ...\n"
            val ois = ObjectInputStream(socket.getInputStream())
            val returnMessage = ois.readObject() as Message

            label.text += "Returned from server: $returnMessage"

            oos.flush()
            oos.close()
            ois.close()
        } catch (e: Exception) {
            println(e)
            label.text += "\n$e"
        } finally {
            try {
                socket?.close()
            } catch (e: Exception) {
                println(e)
            }
        }
    }
}
