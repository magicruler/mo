extern crate wgpu;

pub struct Graphics{
    adapter: wgpu::Adapter,
    pub device: wgpu::Device,
    pub queue: wgpu::Queue,
}

impl Graphics{
    pub fn new()->Self{
        let adapter = wgpu::Adapter::request(&wgpu::RequestAdapterOptions {
            power_preference: wgpu::PowerPreference::HighPerformance, 
            backends:  wgpu::BackendBit::PRIMARY,
        }).unwrap();

        let (device, queue) = adapter.request_device(&wgpu::DeviceDescriptor {
            extensions: wgpu::Extensions {
                anisotropic_filtering: true,
            },
            limits: wgpu::Limits::default(),
        });

        return Graphics{adapter:adapter, device:device, queue:queue};
    }
}