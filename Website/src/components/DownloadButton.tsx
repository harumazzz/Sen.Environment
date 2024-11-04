import { Button, SxProps, Theme } from "@mui/material";

const DownloadButton: React.FC<{ handleDownload: () => void; sx: SxProps<Theme> | undefined }> = ({ handleDownload, sx }) => {
    return (
        <Button onClick={handleDownload} variant="contained" color="primary" sx={{ borderRadius: "8px", ...sx }}>
            Download
        </Button>
    );
};

export default DownloadButton;
