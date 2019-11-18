use std::net::TcpStream;
use std::io::{Read, Write};

fn main() {

    let message = String::from("Rust");
    let mut stream = TcpStream::connect(("localhost", 33333)).unwrap();
    

    stream.write(message.as_bytes()).unwrap();
    println!("Message has been sent");

    let mut data = vec![0 as u8; 256];
	
    match stream.read(&mut data) {
	
        Ok(size) => {
            let answ = String::from_utf8_lossy(&data[..size]);
			
            println!("\nReceived message: {}",answ);
        },
        Err(e) => {
            println!("Error: {}", e);
        }
		
    }
}
