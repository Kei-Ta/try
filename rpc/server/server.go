package main

import (
	"fmt"
	"net"
	"net/rpc"
)

// サービス構造体
type Arith int

// サービスメソッド
func (t *Arith) Multiply(args *Args, reply *int) error {
	*reply = args.A * args.B
	return nil
}

func (t *Arith) Multiply2(args *Args, reply *int) error {
	*reply = args.A * args.B * 2
	return nil
}

// 引数構造体
type Args struct {
	A, B int
}

func main() {
	arith := new(Arith)
	rpc.Register(arith)

	listener, err := net.Listen("tcp", ":1234")
	if err != nil {
		fmt.Println("Error listening:", err)
		return
	}

	fmt.Println("Server is listening on port 1234...")
	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Error accepting connection:", err)
			continue
		}
		go rpc.ServeConn(conn)
	}
}
