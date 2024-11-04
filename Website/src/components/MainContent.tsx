import React, { useState, useEffect } from "react";
import { Typography, Button, Box, useMediaQuery, Snackbar } from "@mui/material";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import senLogo from "../assets/logo.ico";
import detectOS from "../utility/Platform";
import { GitHubAPI } from "../models/GitHub";
import DownloadButton from "./DownloadButton";

const MainContent: React.FC<{ setActivePage: (value: string) => void }> = () => {
    const prefersDarkMode = useMediaQuery("(prefers-color-scheme: dark)");
    const allowance: Array<string> = ["windows", "android"];
    const [message, setMessage] = useState<string>("");
    const [open, setOpen] = useState<boolean>(false);
    const includeOS = allowance.includes(detectOS());
    const [downloadCount, setDownloadCount] = useState<number>(0);

    useEffect(() => {
        const link: string = "https://api.github.com/repos/Haruma-VN/Sen.Environment/releases/tags/release";
        const fetchData = async () => {
            const response = await fetch(link, {
                method: "GET",
                headers: { "Content-Type": "application/json" },
            });
            if (!response.ok) {
                throw new Error("An error found when fetching url, retry later");
            }
            return (await response.json()) as GitHubAPI;
        };
        fetchData()
            .then((data) => setDownloadCount(data.assets.reduce((a, b) => a + b.download_count, 0)))
            .catch((e) => {
                setMessage(e.message);
                handleOpenDialog();
            });
    }, []);

    const handleOpenDialog = () => {
        setOpen(true);
    };

    const handleClose = (_?: React.SyntheticEvent | Event, reason?: string) => {
        if (reason === "clickaway") {
            return;
        }
        setOpen(false);
    };

    const handleDownload = () => {
        const link: string = "https://api.github.com/repos/Haruma-VN/Sen.Environment/releases/tags/release";
        const download = async () => {
            const response = await fetch(link, {
                method: "GET",
                headers: { "Content-Type": "application/json" },
            });
            if (!response.ok) {
                throw new Error("An error found when fetching url, retry later");
            }
            const data = (await response.json()) as GitHubAPI;
            let baseUrl = "";
            switch (detectOS()) {
                case "windows":
                    baseUrl = data.assets.find((e) => e.name === "win-x64.zip")!.browser_download_url;
                    break;
                case "android":
                    baseUrl = data.assets.find((e) => e.name === "android-arm64-v8a.zip")!.browser_download_url;
                    break;
                default:
                    throw new Error("Current operating system does not supported");
            }
            window.location.href = baseUrl;
            setDownloadCount(downloadCount + 1);
        };
        download()
            .then(() => setMessage("Download success, enjoy the tool!"))
            .catch((e) => setMessage(e.message))
            .finally(() => handleOpenDialog());
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
        <React.Fragment>
            <ThemeProvider theme={theme}>
                <Box sx={{ padding: 3, maxWidth: "800px", margin: "auto", textAlign: "center" }}>
                    <Box sx={{ marginBottom: 4 }}>
                        <img src={senLogo} alt="Sen Logo" style={{ maxHeight: "200px", width: "auto" }} />
                        <Typography variant="h3" sx={{ fontWeight: "bold", mt: 2 }}>
                            Sen
                        </Typography>
                        <Typography variant="h6" color="text.secondary" sx={{ mt: 1 }}>
                            The PvZ2-Modding tool created by Haruma, recommended by most modders.
                        </Typography>
                    </Box>
                    <Box sx={{ marginBottom: 2 }}>
                        <Typography variant="body1" color="text.secondary" sx={{ mb: 1 }}>
                            Version: <strong>4.0</strong>
                        </Typography>
                        <Typography variant="body1" color="text.secondary">
                            Your operating system is <strong>{detectOS()}</strong> and is <strong>{includeOS ? "supported for download" : "unsupported at the moment"}</strong>.
                        </Typography>
                        {detectOS() === "windows" ? (
                            <Typography variant="body2" sx={{ mt: 2 }} color="text.secondary">
                                If you having any issues with Sen on your machine, try to install{" "}
                                <a style={{ color: "blue", textDecoration: "none", fontStyle: "italic" }} href="https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170">
                                    Microsoft Visual C++ Redistributable 2015 and 2017
                                </a>{" "}
                                on your machine. You can join{" "}
                                <a style={{ color: "#5865F2", textDecoration: "none", fontStyle: "italic" }} href="https://discord.gg/C2Xr2kaBYJ">
                                    our discord server
                                </a>{" "}
                                for further assistant.
                            </Typography>
                        ) : (
                            <></>
                        )}
                        <Typography variant="body1" color="text.secondary" sx={{ mt: 1 }}>
                            Download count: <strong>{downloadCount}</strong>
                        </Typography>
                    </Box>
                    <Box sx={{ display: "flex", justifyContent: "center", alignItems: "center", gap: 2, flexDirection: { xs: "column", sm: "row" }, mt: 4 }}>
                        {includeOS ? (
                            <DownloadButton handleDownload={handleDownload} sx={{ backgroundColor: "green", padding: "8px 24px", fontSize: "1.1rem", width: { xs: "100%", sm: "auto" } }} />
                        ) : (
                            <Button variant="outlined" sx={{ padding: "8px 24px", fontSize: "1.1rem", width: { xs: "100%", sm: "auto" } }} disabled>
                                Unsupported
                            </Button>
                        )}
                        <Button
                            variant="contained"
                            sx={{ borderRadius: "8px", padding: "8px 24px", fontSize: "1.1rem", width: { xs: "100%", sm: "auto", backgroundColor: "#5865F2" } }}
                            onClick={() => window.open("https://discord.gg/C2Xr2kaBYJ", "_blank")}
                        >
                            Discord Server
                        </Button>
                    </Box>
                    <Snackbar
                        open={open}
                        autoHideDuration={6000}
                        onClose={handleClose}
                        message={message}
                        anchorOrigin={{ vertical: "bottom", horizontal: "center" }}
                        action={
                            <Button color="secondary" size="small" onClick={handleClose}>
                                Close
                            </Button>
                        }
                        sx={{
                            "& .MuiSnackbarContent-root": {
                                borderRadius: "8px",
                                backgroundColor: theme.palette.primary.main,
                                color: theme.palette.primary.contrastText,
                            },
                        }}
                    />
                </Box>
            </ThemeProvider>
        </React.Fragment>
    );
};

export default MainContent;
