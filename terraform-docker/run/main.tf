terraform {
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "~> 2.13.0"
    }
  }
}

provider "docker" {}

resource "docker_image" "nginx" {
  name         = "nginx:latest"
  force_remove = true
}

resource "docker_container" "nginx" {
  image = docker_image.nginx.latest
  name  = "nginx_container"
  ports {
    internal = 80
    external = 8081
  }
}

provider "local" {}

resource "local_file" "example_aaaaa" {
  content  = "Hello, Terraform!"
  filename = "${path.module}/hello.txt"
}

resource "null_resource" "example" {
  provisioner "local-exec" {
    command = "echo Hello, World!"
  }
}