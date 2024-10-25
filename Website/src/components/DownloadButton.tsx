import { Button } from "@mui/material";

const DownloadButton: React.FC<{ handleDownload: () => void }> = ({ handleDownload }) => {
    return (
        <Button onClick={handleDownload} variant="contained" color="primary" sx={{ borderRadius: "8px", marginTop: 20 }}>
            Download
        </Button>
    );
};

export default DownloadButton;
