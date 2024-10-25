import React, { useState } from "react";
import { Container, CssBaseline, useMediaQuery, Box } from "@mui/material";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import MainContent from "./components/MainContent";
import Footer from "./components/Footer";
import Navbar from "./components/Navbar";
import SidebarDrawer from "./components/SidebarDrawer";
import Changelog from "./components/Changelog";
import About from "./components/About";
import BottomNavBar from "./components/BottomNavbar";
import Tutorial from "./components/Tutorial";

const App: React.FC = () => {
    const isLargeScreen = useMediaQuery("(min-width:1024px)");
    const [isDrawerOpen, setIsDrawerOpen] = useState(isLargeScreen);
    const [activePage, setActivePage] = useState("Home");
    const prefersDarkMode = useMediaQuery("(prefers-color-scheme: dark)");
    const isSmallScreen = useMediaQuery("(max-width:600px)");

    const toggleDrawer = () => {
        setIsDrawerOpen(!isDrawerOpen);
    };

    const renderPageContent = () => {
        switch (activePage) {
            case "Home":
                return <MainContent setActivePage={setActivePage} />;
            case "Changelog":
                return <Changelog />;
            case "About":
                return <About />;
            case "Tutorial":
                return <Tutorial />;
            default:
                return <MainContent setActivePage={setActivePage} />;
        }
    };

    const theme = React.useMemo(
        () =>
            createTheme({
                palette: {
                    mode: prefersDarkMode ? "dark" : "light",
                },
            }),
        [prefersDarkMode],
    );

    return (
        <ThemeProvider theme={theme}>
            <React.Fragment>
                <CssBaseline />
                <Box sx={{ display: "flex", flexDirection: "column", minHeight: "100vh", flexGrow: 1 }}>
                    <Box sx={{ display: "flex", flex: 1 }}>
                        {isLargeScreen && <SidebarDrawer isDrawerOpen={isDrawerOpen} toggleDrawer={toggleDrawer} setActivePage={setActivePage} />}
                        <Box sx={{ flexGrow: 1 }}>
                            <Navbar />
                            <Container component="main" sx={{ flexGrow: 1, my: 2 }}>
                                {renderPageContent()}
                            </Container>
                        </Box>
                    </Box>
                    {!isSmallScreen && <Footer />}
                </Box>
            </React.Fragment>
            <BottomNavBar setActivePage={setActivePage} />
        </ThemeProvider>
    );
};

export default App;
