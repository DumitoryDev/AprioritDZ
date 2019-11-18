use std::net::TcpListener;
use std::io::{Read, Write};

fn main() {
    
    let tcp_listener = TcpListener::bind(("localhost", 33333)).unwrap();
	println!("Server running!");
	
    let mut buf = vec![0 as u8; 256];
   
	for stream in tcp_listener.incoming(){ //Many connections
		match stream{
		
			Ok(mut stream)=>{
			
				match stream.read(&mut buf) {
				
					Ok(size) => {
					
						let buf_name = String::from_utf8_lossy(&buf[..size]);
						let answer = "Hello, ".to_string() + &buf_name;

						stream.write(answer.as_bytes()).unwrap();
						println!("Message sended to {}", buf_name);
						
					},
					
					Err(e) => {
						println!("Error: {}", e);
					}
				}
			}
			
			Err(e)=>{
				println!("Error: {}", e);
			}
		}
	}
    

}
