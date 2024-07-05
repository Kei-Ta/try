terraform {
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "9.9.9"
    }
  }
}

provider "docker" {}

resource "docker_image" "nginx" {
  name = "nginx:latest"
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