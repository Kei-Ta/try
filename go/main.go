package main

import (
    "fmt"
    "net/http"
    "github.com/gorilla/mux"
)

func helloWorldHandler(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello, World!")
}

func main() {
    r := mux.NewRouter()
    r.HandleFunc("/", helloWorldHandler).Methods("GET")

    fmt.Println("Starting server at port 8082")
    if err := http.ListenAndServe(":8082", r); err != nil {
        fmt.Println(err)
    }
}
