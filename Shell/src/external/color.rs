pub mod sen {

    pub mod shell {
        pub enum Color {
            Red,
            Green,
            Cyan,
            Yellow,
            Default,
        }

        pub fn exchange_color (
            value: &str,
        ) -> Color
        {
            match value {
                "red" => Color::Red,
                "green" => Color::Green,
                "cyan" => Color::Cyan,
                "yellow" => Color::Yellow,
                _ => Color::Default,
            }
        }
    }

}