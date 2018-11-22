package pojo

import java.io.Serializable
import java.net.InetSocketAddress

data class Message(val header: Header, val body: String): Serializable

data class Header(val clientAddress: InetSocketAddress, val serverAddress: InetSocketAddress): Serializable