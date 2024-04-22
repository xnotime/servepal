use std::{thread, time::Duration};

use serialport;

const PORT_PATH: &'static str = "/dev/ttyUSB0";
const BAUD: u32 = 115_200;

fn main() {
    let mut port = serialport::new(PORT_PATH, BAUD)
        .timeout(Duration::from_secs(2))
        .open()
        .expect("USB device not found");
    let mut row = 0;
    loop {
        _ = port.write(b"w\nc\n");
        _ = port.write(&[b'r', row, b'\n']);
        _ = port.write(&[b'd', 0b01000010, b'\n']);
        row = (row + 1) % 8;
        thread::sleep(Duration::from_secs(2));
    }
}
