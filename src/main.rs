extern crate log;
extern crate winit;
extern crate wgpu;

mod graphics;
use graphics::Graphics;

fn main() {
    use winit::{
        event,
        event_loop::{ControlFlow, EventLoop},
    };

    env_logger::init();
    let event_loop = EventLoop::new();

    let (window, size, surface) = {
        let window = winit::window::Window::new(&event_loop).unwrap();
        let size = window.inner_size();

        let surface = wgpu::Surface::create(&window);
        (window, size, surface)
    };

    let mut graphics = Graphics::new();

    let vs = include_bytes!("shader.vert.spv");
    let vs_module =
    graphics.device.create_shader_module(&wgpu::read_spirv(std::io::Cursor::new(&vs[..])).unwrap());

    let fs = include_bytes!("shader.frag.spv");
    let fs_module =
    graphics.device.create_shader_module(&wgpu::read_spirv(std::io::Cursor::new(&fs[..])).unwrap());

    let bind_group_layout =
    graphics.device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor { bindings: &[] });
    let bind_group = graphics.device.create_bind_group(&wgpu::BindGroupDescriptor {
        layout: &bind_group_layout,
        bindings: &[],
    });
    let pipeline_layout = graphics.device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
        bind_group_layouts: &[&bind_group_layout],
    });

    let render_pipeline = graphics.device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
        layout: &pipeline_layout,
        vertex_stage: wgpu::ProgrammableStageDescriptor {
            module: &vs_module,
            entry_point: "main",
        },
        fragment_stage: Some(wgpu::ProgrammableStageDescriptor {
            module: &fs_module,
            entry_point: "main",
        }),
        rasterization_state: Some(wgpu::RasterizationStateDescriptor {
            front_face: wgpu::FrontFace::Ccw,
            cull_mode: wgpu::CullMode::None,
            depth_bias: 0,
            depth_bias_slope_scale: 0.0,
            depth_bias_clamp: 0.0,
        }),
        primitive_topology: wgpu::PrimitiveTopology::TriangleList,
        color_states: &[wgpu::ColorStateDescriptor {
            format: wgpu::TextureFormat::Bgra8UnormSrgb,
            color_blend: wgpu::BlendDescriptor::REPLACE,
            alpha_blend: wgpu::BlendDescriptor::REPLACE,
            write_mask: wgpu::ColorWrite::ALL,
        }],
        depth_stencil_state: None,
        index_format: wgpu::IndexFormat::Uint16,
        vertex_buffers: &[],
        sample_count: 1,
        sample_mask: !0,
        alpha_to_coverage_enabled: false,
    });

    let mut swap_chain = graphics.device.create_swap_chain(
        &surface,
        &wgpu::SwapChainDescriptor {
            usage: wgpu::TextureUsage::OUTPUT_ATTACHMENT,
            format: wgpu::TextureFormat::Bgra8UnormSrgb,
            width: size.width,
            height: size.height,
            present_mode: wgpu::PresentMode::Vsync,
        },
    );

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Poll;
        match event {
            event::Event::MainEventsCleared => window.request_redraw(),
            event::Event::RedrawRequested(_) => {
                let frame = swap_chain
                    .get_next_texture();
                    // .expect("Timeout when acquiring next swap chain texture");
                let mut encoder =
                graphics.device.create_command_encoder(&wgpu::CommandEncoderDescriptor { todo: 0 });
                {
                    let mut rpass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                        color_attachments: &[wgpu::RenderPassColorAttachmentDescriptor {
                            attachment: &frame.view,
                            resolve_target: None,
                            load_op: wgpu::LoadOp::Clear,
                            store_op: wgpu::StoreOp::Store,
                            clear_color: wgpu::Color::GREEN,
                        }],
                        depth_stencil_attachment: None,
                    });
                    rpass.set_pipeline(&render_pipeline);
                    rpass.set_bind_group(0, &bind_group, &[]);
                    rpass.draw(0 .. 3, 0 .. 1);
                }

                graphics.queue.submit(&[encoder.finish()]);
            }
            event::Event::WindowEvent {
                event: event::WindowEvent::CloseRequested,
                ..
            } => *control_flow = ControlFlow::Exit,
            _ => {}
        }
    });
}