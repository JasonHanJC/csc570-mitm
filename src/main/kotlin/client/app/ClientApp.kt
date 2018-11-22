package client.app

import tornadofx.*
import client.view.ClientView

class ClientApp: App(ClientView::class)

fun main(args: Array<String>) {
    launch<ClientApp>(args)
}