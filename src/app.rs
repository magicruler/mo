use graphics::Graphics;

struct App{
    graphics:Option<Graphics>,
}

impl App{
    pub new()->Self{
        return App{graphics:None};
    }
    pub init(&self){
        
    }

    pub main_loop(){

    }
}