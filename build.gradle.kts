import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

plugins {
    kotlin("jvm") version "1.3.10"
}

group = "ITU"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    compile(kotlin("stdlib-jdk8"))
    compile("no.tornado:tornadofx:1.7.17")
}

tasks.withType<KotlinCompile> {
    kotlinOptions.jvmTarget = "1.8"
}