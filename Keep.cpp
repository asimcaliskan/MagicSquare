#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace cv;
using namespace std;

void convertToBayer(const Mat& src, Mat& bayer) {
    // Ensure the source image is in BGR format
    assert(src.type() == CV_8UC3);

    // Create an empty Bayer pattern image
    bayer = Mat::zeros(src.rows, src.cols, CV_8UC1);

    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            if (y % 2 == 0) {
                if (x % 2 == 0) {
                    bayer.at<uchar>(y, x) = src.at<Vec3b>(y, x)[2]; // Red
                } else {
                    bayer.at<uchar>(y, x) = src.at<Vec3b>(y, x)[1]; // Green
                }
            } else {
                if (x % 2 == 0) {
                    bayer.at<uchar>(y, x) = src.at<Vec3b>(y, x)[1]; // Green
                } else {
                    bayer.at<uchar>(y, x) = src.at<Vec3b>(y, x)[0]; // Blue
                }
            }
        }
    }
}

int main() {
    // Initialize camera
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera" << endl;
        return -1;
    }

    // Create a window to display the frame
    namedWindow("Bayer Pattern", WINDOW_AUTOSIZE);

    // Capture a frame
    Mat frame;
    cap >> frame;
    if (frame.empty()) {
        cerr << "Error: Could not capture frame" << endl;
        return -1;
    }

    // Convert to Bayer format
    Mat bayer;
    convertToBayer(frame, bayer);

    // Display the Bayer pattern image
    imshow("Bayer Pattern", bayer);
    waitKey(0);

    // Socket communication setup
    int sock = 0;
    struct sockaddr_in serv_addr;
    char* server_ip = "192.168.1.2"; // Replace with the IP address of the IMX8MP board
    int server_port = 8080; // Replace with the appropriate port number

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error: Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        cerr << "Error: Invalid address/ Address not supported" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error: Connection failed" << endl;
        return -1;
    }

    // Send the Bayer pattern data
    int bytes_sent = send(sock, bayer.data, bayer.total() * bayer.elemSize(), 0);
    if (bytes_sent < 0) {
        cerr << "Error: Failed to send data" << endl;
        return -1;
    }

    cout << "Sent " << bytes_sent << " bytes to the server" << endl;

    // Close the socket
    close(sock);

    return 0;
}
