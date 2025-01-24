pub mod sen {

    pub mod launcher {

        pub fn construct_arguments (

        ) -> Vec<String>
        {
            std::env::args().skip(1).collect::<Vec<String>>()
        }

    }

}