import { Card, CardContent, Typography, Stack, CardMedia, Box } from "@mui/material";

const tutorials = [
    {
        title: "PvZ2 Modding: How to modify PvZ2 Animation?",
        description: "How to effectively use animations with the latest features in Sen.",
        videoUrl: "https://www.youtube.com/embed/fSXOXuL7sUo",
    },
];

const Tutorial = () => {
    return (
        <Stack spacing={3}>
            {tutorials.map((tutorial, index) => (
                <Card key={index} sx={{ p: 2, boxShadow: 3, borderRadius: 2 }}>
                    <CardContent>
                        <Typography variant="h6" sx={{ fontWeight: "bold", mb: 1 }}>
                            {tutorial.title}
                        </Typography>
                        <Typography variant="body2" color="textSecondary" sx={{ mb: 2 }}>
                            {tutorial.description}
                        </Typography>
                        <Box
                            sx={{
                                position: "relative",
                                paddingTop: "56.25%",
                                overflow: "hidden",
                                borderRadius: "8px",
                                boxShadow: 1,
                            }}
                        >
                            <CardMedia
                                component="iframe"
                                src={tutorial.videoUrl}
                                allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                                allowFullScreen
                                sx={{
                                    position: "absolute",
                                    top: 0,
                                    left: 0,
                                    width: "100%",
                                    height: "100%",
                                    borderRadius: "8px",
                                    border: "none",
                                }}
                            />
                        </Box>
                    </CardContent>
                </Card>
            ))}
        </Stack>
    );
};

export default Tutorial;
