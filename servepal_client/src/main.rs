use std::time::Duration;

use serialport;

const PORT_PATH: &'static str = "/dev/ttyUSB0";

fn main() {
    let port = serialport::new(PORT_PATH, 115_200)
        .timeout(Duration::from_secs(2))
        .open()
        .expect("USB device not found");
    Ok(())
}
