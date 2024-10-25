import { Button } from "@mui/material";
import { useEffect, useState } from "react";

const CountdownButton = () => {
    const [timeLeft, setTimeLeft] = useState<string>("");

    useEffect(() => {
        const targetTimeGMT7 = new Date("2024-10-26T00:00:00+07:00");

        const exchanger = (time: number): string => {
            return time < 10 ? `0${time}` : `${time}`;
        };

        const updateCountdown = () => {
            const currentDate = new Date();
            const localOffset = currentDate.getTimezoneOffset() * 60000;
            const gmt7Offset = 7 * 60 * 60000;
            const gmt7Date = new Date(currentDate.getTime() + gmt7Offset + localOffset);
            const timeDiff = targetTimeGMT7.getTime() - gmt7Date.getTime();

            if (timeDiff > 0) {
                const days = Math.floor(timeDiff / (1000 * 60 * 60 * 24));
                const hours = Math.floor((timeDiff % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
                const minutes = Math.floor((timeDiff % (1000 * 60 * 60)) / (1000 * 60));
                const seconds = Math.floor((timeDiff % (1000 * 60)) / 1000);
                setTimeLeft(`Available in ${exchanger(days)} days ${exchanger(hours)} hours ${exchanger(minutes)} minutes ${exchanger(seconds)} seconds`);
            } else {
                setTimeLeft("Download Available");
                clearInterval(interval);
            }
        };

        const interval = setInterval(updateCountdown, 1000);

        return () => clearInterval(interval);
    }, []);

    return (
        <Button variant="outlined" sx={{ borderRadius: "8px", mt: 3 }} disabled={timeLeft !== "Download Available"}>
            {timeLeft !== "Download Available" ? `${timeLeft}` : "Download"}
        </Button>
    );
};

export default CountdownButton;
