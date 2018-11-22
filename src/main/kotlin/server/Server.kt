package server

import pojo.Header
import pojo.Message
import java.io.ObjectInputStream
import java.io.ObjectOutputStream
import java.lang.Exception
import java.net.InetAddress
import java.net.InetSocketAddress
import java.net.ServerSocket


fun main(args: Array<String>) {
    val listeningPort = 50000
    ServerSocket(listeningPort).use {
        println("Server listening on port ${it.localPort}")
        while (true) {
            it.accept().use {
                val ois = ObjectInputStream(it.getInputStream())
                val recMessage = ois.readObject() as Message
                println("Received message: $recMessage")

                val targetAddress = recMessage.header.clientAddress.hostName

                val message = Message(
                    Header(
                        InetSocketAddress(InetAddress.getLocalHost().getHostAddress(), it.localPort),
                        InetSocketAddress(targetAddress, it.port)
                    ),
                    "Received this message: ${recMessage.body}"
                )
                val oos = ObjectOutputStream(it.getOutputStream())
                oos.writeObject(message)
            }
        }
    }
}