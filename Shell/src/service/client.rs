pub mod sen {

    pub mod shell {

        pub trait Client {

            fn initialize(&mut self) -> ();

            fn execute(&mut self) -> ();

        }

    }

}