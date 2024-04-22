use std::{thread, time::Duration};

use serialport;
use psutil::cpu::CpuPercentCollector;

const PORT_PATH: &'static str = "/dev/ttyUSB0";
const BAUD: u32 = 115_200;

fn main() {
    let mut port = serialport::new(PORT_PATH, BAUD)
        .timeout(Duration::from_secs(2))
        .open()
        .expect("USB device not found");
    let mut pct = CpuPercentCollector::new()
        .expect("Cannot collect CPU%");
    let mut row = 0;
    loop {
        _ = port.write(b"w\nc\n");
        _ = port.write(&[b'r', row, b'\n']);
        _ = port.write(&[b'd', 0b01000010, b'\n']);
        if let Ok(cpu_pct) = pct.cpu_percent() {
            let fan_speed: f32 = compute_fan_curve(cpu_pct / 100.);
            let fan_byte = (fan_speed * 256.) as u8;
            _ = port.write(&[b'f', fan_byte, b'\n']);
        }
        row = (row + 1) % 8;
        thread::sleep(Duration::from_secs(2));
    }
}

const CPU_MIN_MAX: f32 = 0.15;

fn compute_fan_curve(cpu_pct: f32) -> f32 {
    if cpu_pct < CPU_MIN_MAX {
        0.
    } else if cpu_pct > (1. - CPU_MIN_MAX) {
        1.
    } else {
        let cpu_pct_shifted =
            (cpu_pct - CPU_MIN_MAX) / (1. - (2. * CPU_MIN_MAX));
        cpu_pct_shifted
    }
}
