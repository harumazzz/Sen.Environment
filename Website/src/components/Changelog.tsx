import { Card, CardContent, Typography, Stack, CardMedia, Box, Divider } from "@mui/material";
import senLogo from "../assets/logo.ico";
// TODO : Add database
const updates = [
    {
        version: "4.0",
        date: "2024-10-26",
        changes: [
            "Removed the RSB for Modding method. Added a RSB Project method, RSB Project are the better version of RSB for Modding, it provides you newer way to processing RSB files",
            "Removed the restriction for large file (2GB+ or more)",
            "Changed the WWise Soundbank method so it can decode the whole bnk file",
            "Animation can now split label. Now you can process huge Animation with the latest version of Sen. If you don't want, you can still use the fully merged label.",
            "Fixed the ETC1 Encoding. Changed the encoding algorithm so that the output quality is improved much better. The quality is similar to Google's etc1 tool.",
            "Major speed improvement and ram usage on functions and fix memory issues with low-end devices.",
            "Added Animation Viewer for Sen: Environment",
            "Removed the Android to iOS RSB script and added Platform Convert Script. Android to iOS RSB only convert one side, but now you have the double conversion",
        ],
        specialThanks: [
            ["JNR", "Spanish translation for Sen 4"],
            ["PPP", "Vietnamese translation for Sen 4"],
            ["Vi", "Beta Tester and Bug tracker when Sen 4 was still in development"],
            ["DT, Stuff26, TheShero, FazeSmuziy", "Beta Tester when Sen 4 was in beta"],
            ["People from Modding community", "Without everyone, Sen cannot be good like today. I cannot express how much thankful I am. Please enjoy the newest update as my thanks to everyone"],
        ],
    },
];

updates.reverse();

const Changelog = () => {
    return (
        <Stack spacing={3}>
            {updates.map((update, index) => (
                <Card key={index} sx={{ display: "flex", p: 2, boxShadow: 3, borderRadius: 2 }}>
                    <CardMedia
                        component="img"
                        image={senLogo}
                        alt="Product Logo"
                        sx={{
                            height: 80,
                            width: 80,
                            objectFit: "cover",
                            borderRadius: "8px",
                            mr: 2,
                        }}
                    />
                    <Box sx={{ flex: 1 }}>
                        <CardContent sx={{ p: 0 }}>
                            <Typography variant="h6" sx={{ fontWeight: "bold", mb: 1 }}>
                                Version: {update.version}
                            </Typography>
                            <Typography variant="body2" color="textSecondary" sx={{ mb: 1 }}>
                                Date: {update.date}
                            </Typography>
                            <Divider sx={{ mb: 1 }} />
                            <Typography variant="h6" sx={{ fontWeight: "bold", mb: 1 }}>
                                Update changes:
                            </Typography>
                            <Stack spacing={1}>
                                {update.changes.map((e, idx) => (
                                    <Typography variant="body2" key={idx} sx={{ pl: 1 }}>
                                        • {e}
                                    </Typography>
                                ))}
                            </Stack>
                            <Divider sx={{ mb: 1, mt: 1 }} />
                            <Typography variant="h6" sx={{ fontWeight: "bold", mb: 1 }}>
                                Special thanks to:
                            </Typography>
                            <Stack spacing={1}>
                                {update.specialThanks.map((e, idx) => (
                                    <Typography variant="body2" key={idx} sx={{ pl: 1 }}>
                                        • {e[0]}: {e[1]}
                                    </Typography>
                                ))}
                            </Stack>
                        </CardContent>
                    </Box>
                </Card>
            ))}
        </Stack>
    );
};

export default Changelog;
