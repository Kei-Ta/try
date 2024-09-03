package main

import (
	"fmt"
	"net/rpc"
)

// 引数構造体
type Args struct {
	A, B int
}

func main() {
	client, err := rpc.Dial("tcp", "localhost:1234")
	if err != nil {
		fmt.Println("Error dialing:", err)
		return
	}

	args := Args{A: 7, B: 8}
	var reply int

	err = client.Call("Arith.Multiply2", &args, &reply)
	if err != nil {
		fmt.Println("Error calling RPC:", err)
		return
	}

	fmt.Printf("Arith: %d*%d=%d\n", args.A, args.B, reply)
}
