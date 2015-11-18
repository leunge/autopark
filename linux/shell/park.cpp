#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <time.h>

#define PI 3.14159265358979323846
#define MAXINT 987654321

using namespace cv;
using namespace std;

typedef vector<int> Node;
typedef vector<Node> NodeList;

int inBetween(int, int, int);
double getCordY(int, double, double);
double getCordX(int, double, double);
double getDistance(int, int, int, int);
Node makeNode(int x, int y, int l1, int end, int l2);
NodeList wayToCord(int x, int y, NodeList nl);


Mat image;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;
int startX = 42, startY = 305;
Size axes(20, 20);



//linedetect 하는함수.
NodeList linedetect(void)
{
	FILE * datafile;
	NodeList nl;
	const string videoStreamAddress = "http://192.168.1.4:8080/?action=snapshot?dummy=param.mjpg";
	VideoCapture cap;
	
	Mat frame;
	Mat frameCopy;
	char next;
	
	for(;;){
		cap.open(videoStreamAddress);
		
		//cvNamedWindow( "resul", 1);

		
		Mat contours;
		
		double grad1, grad2, yinter1, yinter2;
		double grad1_inv, grad2_inv, yinter1_inv, yinter2_inv;
		int l1x1, l1x2, l1y1, l1y2;
		int l2x1, l2x2, l2y1, l2y2;
		int x, y;
		int a, b, c, d;
		cap >> frame;
		//frame = imread("map.png", 1);
		frame.copyTo( frameCopy );	
		Canny(frameCopy, contours, 125, 350);
		
		vector<Vec4i> lines;
		HoughLinesP(contours, lines, 1, PI/180, 100, 30, 1000);

		Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
		cout<< "Lines detected: " << lines.size() << endl;
		
		vector<Vec4i>::iterator it1 = lines.begin();
		Point pt1((*it1)[0], (*it1)[1]);	
		Point pt2((*it1)[2], (*it1)[3]);
		//line(frameCopy, pt1, pt2, Scalar(0), 3);
		vector<Vec4i>::iterator it2;	

		while( it1 != lines.end() )
		{						
			// 라인 1
			l1x1 = (*it1)[0];
			l1y1 = (*it1)[1];
			l1x2 = (*it1)[2];
			l1y2 = (*it1)[3];
			
			// y = grad1 * x + yinter
			// x = (y - yinter) / grad1
			grad1 = (double)(l1y2 - l1y1) / (double) (l1x2 - l1x1);
			yinter1 = (double)(l1y1 * l1x2 - l1x1 * l1y2) / (double) (l1x2 - l1x1);

			grad1_inv = (double)(l1x2 - l1x1) / (double) (l1y2 - l1y1);
			yinter1_inv = (double)(l1x1 * l1y2 - l1y1 * l1x2) / (double) (l1y2 - l1y1);
			
			it2 = it1;
			it2++;

			while(it2 != lines.end()){
				// 라인 2
				l2x1 = (*it2)[0];
				l2y1 = (*it2)[1];
				l2x2 = (*it2)[2];
				l2y2 = (*it2)[3];
				grad2 = (double)(l2y2 - l2y1) / (double) (l2x2 - l2x1);
				yinter2 = (double)(l2y1 * l2x2 - l2x1 * l2y2) / (double) (l2x2 - l2x1);

				grad2_inv = (double)(l2x2 - l2x1) / (double) (l2y2 - l2y1);
				yinter2_inv = (double)(l2x1 * l2y2 - l2y1 * l2x2) / (double) (l2y2 - l2y1);
				
				//비슷한 라인이면 지운다.
				if(abs(getCordY((l2x1 + l2x2) / 2, grad1, yinter1) - getCordY((l2x1 + l2x2) / 2, grad2, yinter2)) < 30){
					// 길이가 긴 걸 남긴다.		
					if(getDistance(l1x1, l1y1, l1x2, l1y2) < getDistance(l2x1, l2y1, l2x2, l2y2)){
						it1--;
						lines.erase(it1 + 1);
						break;
					} else {
						it2--;
						lines.erase(it2 + 1);
						it2++;
						continue;
					}
				
				}

				// x, y inversed, 비슷한 라인이면 지운다.
				if(abs(getCordY((l2y1 + l2y2) / 2, grad1_inv, yinter1_inv) - getCordY((l2y1 + l2y2) / 2, grad2_inv, yinter2_inv)) < 30){
					// 길이가 긴 걸 남긴다.		
					if(getDistance(l1y1, l1x1, l1y2, l1x2) < getDistance(l2y1, l2x1, l2y2, l2x2)){
						it1--;
						lines.erase(it1 + 1);
						break;
					} else {
						it2--;
						lines.erase(it2 + 1);
						it2++;
						continue;
					}
				
				}
			
				if(abs(getCordX((l2y1 + l2y2) / 2, grad1, yinter1) - getCordX((l2y1 + l2y2) / 2, grad2, yinter2)) < 50){
					cout<<"line dumped by X diff"<<endl;
					// 길이가 긴 걸 남긴다.		
					if(getDistance(l1x1, l1y1, l1x2, l1y2) < getDistance(l2x1, l2y1, l2x2, l2y2)){
						it1--;
						lines.erase(it1 + 1);
						break;
					} else {
						it2--;
						lines.erase(it2 + 1);
						it2++;
						continue;
					}
				}	
				it2++;
			}
			
			it1++;
		}
		
		// 지워진 라인 확인
		it1 = lines.begin();
		int line_num = 0;
		while(it1 != lines.end()){
			
			l2x1 = (*it1)[0];
			l2y1 = (*it1)[1];
			l2x2 = (*it1)[2];
			l2y2 = (*it1)[3];
			grad2 = (double)(l2y2 - l2y1) / (double) (l2x2 - l2x1);
			yinter2 = (double)(l2y1 * l2x2 - l2x1 * l2y2) / (double) (l2x2 - l2x1);
			cout << "line_num : " << line_num << " : y = " << grad2 << "x + " << yinter2 << endl;
			
			Point pt1((*it1)[0], (*it1)[1]);	
			Point pt2((*it1)[2], (*it1)[3]);
			line(frameCopy, pt1, pt2, Scalar(0, 0, 255), 2);
			it1++;
			line_num++;
		}
		cout << line_num << " lines left" << endl;
		
		// 만나는 점 표시
		it1 = lines.begin();
		int l1_index = 0;
		int l2_index = 0;
		int l1_other = -1;
		int grad1Inf = 0;
		int grad2Inf = 0;
		
		while(it1 != lines.end()){
			grad1Inf = 0;
			
			l1x1 = (*it1)[0];
			l1y1 = (*it1)[1];
			l1x2 = (*it1)[2];
			l1y2 = (*it1)[3];
			grad1 = (double)(l1y2 - l1y1) / (double) (l1x2 - l1x1);
			yinter1 = (double)(l1y1 * l1x2 - l1x1 * l1y2) / (double) (l1x2 - l1x1);
		
			if(grad1 > MAXINT || grad1 < -1 * MAXINT)	
				grad1Inf = 1;
			
			it2 = it1;
			l2_index = l1_index;
			it2++;
			l2_index++;
			while(it2 != lines.end()){
				grad2Inf = 0;
				
				// 라인 2
				l2x1 = (*it2)[0];
				l2y1 = (*it2)[1];
				l2x2 = (*it2)[2];
				l2y2 = (*it2)[3];
				
				// 라인의 한 쪽 끝, 1번 라인은 제외돼 있는 상태
				Point x1y1(l2x1, l2y1);
				Size size(30, 30);
				//ellipse(frameCopy, x1y1, size, 0, 0, 360, Scalar(100), 1);
				
				grad2 = (double)(l2y2 - l2y1) / (double) (l2x2 - l2x1);
				yinter2 = (double)(l2y1 * l2x2 - l2x1 * l2y2) / (double) (l2x2 - l2x1);
				
				if(grad2 > MAXINT || grad2 < -1 * MAXINT)	
					grad2Inf = 1;

				
				if(grad1Inf || grad2Inf){

					// x = l1x1 (=~ l1x2)
					if(grad1Inf){
						x = l1x1;
						y = x * grad2 + yinter2;		
					} 
					// x = l2x1 (=~ l2x2)
					else if(grad2Inf){
						x = l2x1;
						y = x * grad1 + yinter1;
						
					}
					if(inBetween(x, l1x1, l1x2) && inBetween(x, l2x1, l2x2)){
						Point pt(x, y);			
						//cout << "gradInf" << endl;
						ellipse(frameCopy, pt, axes, 0, 0, 360, Scalar(255), 3);
						Node node = makeNode(x, y, l1_index, 0, l2_index);
						nl.insert(nl.end(), node);

						// 반대편 지점이 그려져야할 라인이 l1인 경우 처리 추가
						cout << x << "|" << y << "\t" ;
						cout << l1_index << "|" << l2_index << endl;
						
						// 만나는 점이 l2x1, l2y1
						if(abs(x - l2x1) < 10 && abs(y - l2y1) < 10){
							cout << "point l2x2" << l2x2 << "\t" << l2y2 << endl;
							Point other(l2x2, l2y2);
							ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
							// other point가 어느 선 위에 있는지 검사
							Node node = makeNode(l2x2, l2y2, -1, 1, l2_index);
							nl.insert(nl.end(), node);
						}
						// l2x2, l2y2일 때
						if(abs(x - l2x2) < 10 && abs(y - l2y2) < 10){
							cout << "point l2x1" << endl;
							Point other(l2x1, l2y1);
							ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
							Node node = makeNode(l2x1, l2y1, -1, 1, l2_index);
							nl.insert(nl.end(), node);
						} 
						// l1x1, l1y1
						if(abs(x - l1x1) < 10 && abs(y - l1y1) < 10){
							cout << "point l1x1" << endl;
							Point other(l1x2, l1y2);
							ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
							Node node = makeNode(l1x2, l1y2, l1_index, 1, -1);
							nl.insert(nl.end(), node);
						}
						// l1x2, l1y2일 때
						if(abs(x - l1x2) < 10 && abs(y - l1y2) < 10){
							cout << "point l1x2" << endl;
							Point other(l1x1, l1y1);
							ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
							Node node = makeNode(l1x1, l1y1, l1_index, 1, -1);
							nl.insert(nl.end(), node);
						} 
					} else {
						it2++;
						l2_index++;	
						continue;
					}				

				}

				// 만나는 점 표시
				x = (yinter2 - yinter1) / (grad1 - grad2);
				y = (grad1 * yinter2 - yinter1 * grad2) / (grad1 - grad2);
				
				if(x == -2147483648 ){
					it2++;
					l2_index++;
					//cout << "x INF" << endl;
					continue;
				}
				// 화면 상에 보여야 하고, 선 위에 있어야 표시
				if(x > 0 && y > 0 && inBetween(x, l1x1, l1x2) && inBetween(x, l2x1, l2x2) && inBetween(y, l1y1, l1y2) && inBetween(y, l2y1, l2y2)){
					Point ptx1(x, y);	
					cout << "not gradInf" << endl;
					ellipse(frameCopy, ptx1, axes, 0, 0, 360, Scalar(255), 3);
					Node node = makeNode(x, y, l1_index, 0, l2_index);
					nl.insert(nl.end(), node);
					//pointInfoList.insert(ptx1, l1_index, 0, l2_index);
				}
				
				// 만나는 점이 l2x1, l2y1일 때 다른 점 표시
				// 양끝이 다 만나는 경우도 고려해야 함
				// 반대편 지점이 그려져야할 라인이 l1인 경우 처리 추가
				cout << x << "|" << y << "\t" ;
				cout << l1_index << "|" << l2_index << endl;
				
				// l2x1, l2y1
				if(abs(x - l2x1) < 10 && abs(y - l2y1) < 10){
					cout << "point l2x2" << endl;
					Point other(l2x2, l2y2);
					ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
					Node node = makeNode(l2x2, l2y2, -1, 1, l2_index);
					nl.insert(nl.end(), node);
				}
				// l2x2, l2y2일 때
				else if(abs(x - l2x2) < 10 && abs(y - l2y2) < 10){
					cout << "point l2x1" << endl;
					Point other(l2x1, l2y1);
					ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
					Node node = makeNode(l2x1, l2y1, -1, 1, l2_index);
					nl.insert(nl.end(), node);
				} 
				// l1x1, l1y1
				else if(abs(x - l1x1) < 10 && abs(y - l1y1) < 10){
					cout << "point l1x1" << endl;
					Point other(l1x2, l1y2);
					ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
					Node node = makeNode(l1x2, l1y2, l1_index, 1, -1);
					nl.insert(nl.end(), node);
				}
				// l1x2, l1y2일 때
				else if(abs(x - l1x2) < 10 && abs(y - l1y2) < 10){
					cout << "point l1x2" << endl;
					Point other(l1x1, l1y1);
					ellipse(frameCopy, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
					Node node = makeNode(l1x1, l1y1, l1_index, 1, -1);
					nl.insert(nl.end(), node);
				} 
				// 
				else {
					cout << "else ??" << endl;
				}			
				it2++;
				l2_index++;
			}		
						
			it1++;
			l1_index++;
		}
		
		// vector에 넣을 때, 줄 번호 인덱스도 같이 넣기
		// 거리 계산해서 경로 표시
		
		NodeList::iterator nlit1 = nl.begin();
		NodeList::iterator nlit2 ;
		
		// 비슷한 점이 좌표로 잡히면 제거
		while(nlit1 != nl.end()){
			nlit2 = nlit1;
			nlit2++;
			while(nlit2 != nl.end()){
				if(abs((*nlit1)[0] - (*nlit2)[0]) < 5 && abs((*nlit1)[1] - (*nlit2)[1]) < 5){
					nlit2--;
					nl.erase(nlit2 + 1);
				}
				nlit2++;
			}
			nlit1++;
		}

		// 출력 테스트
		NodeList::iterator nlIT = nl.begin();
		int testint = 0;
		while(nlIT != nl.end()){
			cout << testint++ << endl;
			Node::iterator nodeIT = (*nlIT).begin();
			while(nodeIT != (*nlIT).end()){
				//cout << testint << endl;
				cout << (*nodeIT) << "\t";
				nodeIT++;
			}
			cout << endl;
			nlIT++;
		}	
		
		
		// 테스트 좌표 : 935, 454
		//Point test(935, 454);
		//Size axestest(40, 40);
		//ellipse(frameCopy, test, axestest, 0, 0, 360, Scalar(0,0,255), 3);
				
		
		imshow("", frameCopy);	
		waitKey(0);

		// 엔터면 끝, 아니면 계속 다시
		printf("get input : ");
		scanf("%c", &next);
		cvDestroyWindow("");
		if(next == '\n'){			
			break;
		}
		else{ // 다시
			scanf("%c", &next);
			nl.clear();
			continue;
		}
		
	}
	
	cout << "out of loop" << endl;
	//경로 보여주는 출력 함수 wayToCord
	//NodeList way = wayToCord(974, 78, nl);
	
	cout << "start writing to file" << endl;
	datafile = fopen("Nodelist.txt", "wb");
	NodeList::iterator nlIT = nl.begin();
	while(nlIT != nl.end()){
		int node[5] = {(*nlIT)[0], (*nlIT)[1], (*nlIT)[2], (*nlIT)[3], (*nlIT)[4]};
		cout << (*nlIT)[0] << (*nlIT)[1] << (*nlIT)[2] << (*nlIT)[3] << (*nlIT)[4] << endl;
		fwrite(node, sizeof *node, 5, datafile);
		nlIT++;
	}
	fclose(datafile);
	
	return nl;
}

double getCordY(int x, double grad, double yinter)
{
	return x * grad + yinter;	
}

double getCordX(int y, double grad, double yinter)
{
	return (double)(y - yinter) / grad;
}

int inBetween(int x, int x1, int x2)
{
	int isInBetween = 0;
	
	cout << x << " is between " << x1 << " and " << x2 << endl;
	
	if(x1 > x2){
		if( (x1 > x || abs(x1 - x) < 10) && ( x > x2 || abs(x - x2) < 10))
			isInBetween = 1;
	} else if(x2 > x1) {
		if((x2 > x || abs(x2 - x) < 10) && ( x > x1 || abs(x - x1) < 10))
			isInBetween = 1;
	} else { // x1 == x2
		if(x == x1 || x == x2 || abs(x1 - x) < 10 || abs(x2 - x) < 10)
			isInBetween = 1;
	}
	
	return isInBetween;
}

double getDistance(int x1, int y1, int x2, int y2)
{
	double value = pow(x1 - x2, 2) + pow(y1 - y2, 2);
	if(value == 0){
		cout << "distance : 0" << endl;
		return 0;
	}
	return value;
	
}

Node makeNode(int x, int y, int l1, int end, int l2)
{
	Node newNode;
	
	newNode.insert(newNode.end(), x);
	newNode.insert(newNode.end(), y);
	newNode.insert(newNode.end(), l1);
	newNode.insert(newNode.end(), end);
	newNode.insert(newNode.end(), l2);
	
	return newNode;
}

//NodeList wayToCord(int x, int y, NodeList nl)
NodeList wayToCord(int parkNum, NodeList nl)
{
	NodeList way;	
	
	int tempX, tempY;
	int line_index = -1;
	double shortDist = 100000000;
	double dist;
	
	int x, y; // 목적지 좌표
	int index = 0;
	
	// startX, startY 보정작업
	// startX, startY와 가장 가까운 점을 시작 위치로
	double dist_for_start1, dist_for_start2 = 1000000000;
	NodeList::iterator it = nl.begin();
	while(it != nl.end()){
		dist_for_start1 = getDistance(startX, startY, (*it)[0], (*it)[1]);
		cout << "distance from startX, startY - " << (*it)[0] << ", " <<(*it)[1] << ": " << dist_for_start1 << endl;
		if(dist_for_start1 < dist_for_start2){
			dist_for_start2 = dist_for_start1;
			tempX = (*it)[0];
			tempY = (*it)[1];
		}
		it++;
	}
	startX = tempX;
	startY = tempY;
	cout << "start cord : " << startX << "\t" << startY << endl;
	Node node = makeNode(startX, startY, -2, -2, -2);
	way.insert(way.end(), node);
	
	// 목적지 parkNum에 맞는 좌표 찾기
	it = nl.begin();
	while(it != nl.end()){
		if((*it)[3] == 1 && (*it)[0] != startX && (*it)[1] != startY ){
			index++;
			if(index == parkNum){
				x = (*it)[0];
				y = (*it)[1];
				break;
			}
		}
		it++;
	}
	
	// startX, startY 는 각 단계의 시작점
	// 목적지인 x, y에 갈 때까지 경로를 찾는다.
	while(!(startX == x && startY == y)){
		it = nl.begin();
		// 시작점의 길 index
		while(it != nl.end()){
			if((*it)[0] == startX && (*it)[1] == startY){
				if((*it)[3] == 1)
					line_index = (*it)[2] != -1 ? (*it)[2] : (*it)[4];
				else if((*it)[3] == 0)
					line_index = (*it)[2] != line_index ? (*it)[2] : (*it)[4];
				//cout << line_index << endl;
				break;
			}			
			it++;
		}
		
		// 그 인덱스의 모든 점들
		// 가장 가까운 점
		it = nl.begin();
		while(it != nl.end()){
			if( (startX != (*it)[0] || startY != (*it)[1]) && (line_index == (*it)[2] || line_index == (*it)[4]) ){
				//if(startX != (*it)[0])
					//cout << "find shortest point\tx : " << (*it)[0] << "\ty : " << (*it)[1];
				dist = getDistance((*it)[0], (*it)[1], x, y);
				//cout << "\tdist : " << shortDist << endl;
				if(dist < shortDist){
					shortDist = dist;
					tempX = (*it)[0];
					tempY = (*it)[1];					
				}
			}
			it++;
		}
		
		// 추가
		startX = tempX;
		startY = tempY;
		//cout << startX << "\t" << startY << endl;
		
		Node node = makeNode(startX, startY, -2, -2, -2);
		way.insert(way.end(), node);
		
	}
	
	//출력 테스트
	//cout << "RESULT TEST------------------------------------" << endl;
	NodeList::iterator wIT = way.begin();
	while(wIT != way.end()){
		//Node::iterator nodeIT = (*wIT).begin();
		int x = (*wIT)[0];
		int y = (*wIT)[1];
		cout << x << "\t" << y << endl;
		//Point other(x,y);
		//ellipse(image, other, axes, 0, 0, 360, Scalar(0, 255, 0), 3);
		//cout << endl;
		wIT++;
	}
	
	return way;
}
////////////////////////여기서부터 camshift

static void onMouse( int event, int x, int y, int, void* )
{

		
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
		// cout<<"x :";
		// cout<<x;
		// cout<<",  y :";
		// cout<<y;
		// cout<<endl;
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
        break;
    }
}

static void help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./camshiftdemo [camera number]\n";

    cout << "\n\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tc - stop the tracking\n"
            "\tb - switch to/from backprojection view\n"
            "\th - show/hide object histogram\n"
            "\tp - pause video\n"
            "To initialize tracking, select the object with mouse\n";
}

const char* keys =
{
    "{1|  | 0 | camera number}"
};
void setStart()
{
	int x = 42;
	int y = 305;
	selectObject = true;
	origin = Point(x,y);
    selection = Rect(x,y,0,0);

}


int main(int argc, const char** argv)
{
	//맵의 점 저장.
	NodeList nl;
	
	FILE * datafile;

   // int startX = 42, startY = 305;
   if(argc == 1) exit(0);
   
   int parkNum = atoi(argv[1]);
   cout << argv[1] << endl;
   //help();
   const string videoStreamAddress = "http://192.168.1.4:8080/?action=stream?dummy=param.mjpg";
   VideoCapture cap;
   
    Rect trackWindow;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;
    //CommandLineParser parser(argc, argv, keys);
	

   //Mat image, frame, frameCopy;
   Mat frameCopy;
   int i = 0;

   cap.open(videoStreamAddress);
   

   if( !cap.isOpened() )
    {
        help();
        cout << "***Could not initialize capturing...***\n";
        cout << "Current parameter's value: \n";
        //parser.printParams();
        return -1;
    }
	
	//cvNamedWindow( "resul", 1);
	//라인찾는 함수실행.
	namedWindow( "Histogram", 0 );
    namedWindow( "CamShift Demo", 0 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

    Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool paused = false;
	
	if(datafile = fopen("Nodelist.txt", "rb")){
		//파일로부터 읽어와서 nl에 저장
		cout << "file exists" << endl;
		int data[5];
		while(!feof(datafile)){
			cout << "reading files" << endl;
			fread(data, sizeof *data, 5, datafile);
			Node node = makeNode(data[0], data[1], data[2], data[3], data[4]);
			cout << data[0]<< data[1] << data[2] << data[3] << data[4] << endl;
			nl.insert(nl.end(), node);
		}
		fclose(datafile);
	}else{
		cout << "no data file"<< endl;
		nl = linedetect();
	}	
	//NodeList way = wayToCord(974, 78, nl);
	NodeList way = wayToCord(parkNum, nl);
	NodeList::iterator wIT = way.begin();
	
	// int now_x = startX;
	// int now_y = startY;
	// int next_x = startX;
	// int next_y = startY;
	int now_x = (*wIT)[0];
	int now_y = (*wIT)[1];
	wIT++;
	int next_x = (*wIT)[0];
	int next_y = (*wIT)[1];
	bool lock = false;
	
	bool first = true;
	int first_size = 30;
	time_t start, end;
	double dif;
	bool time_s = true;
	int settime = 5;
	
	origin = Point(now_x+first_size,now_y+first_size);//시작 좌표
    selection = Rect(now_x+first_size,now_y+first_size,0,0);
	selection.x = MIN(now_x-first_size > 0? now_x-first_size:0 , origin.x);
	selection.y = MIN(now_y-first_size > 0? now_y-first_size:0, origin.y);
	selection.width = abs(selection.x - origin.x);
	selection.height = abs(selection.y  - origin.y);
	//selection &= Rect(0, 0, image.cols, image.rows);
	time (&start);

   for(;;)
   {
      if( !paused )
        {
            cap >> frame;
            if( frame.empty() )
                break;
        }
			
		
		//frame = imread("map.png", 1);
		frame.copyTo(image);
		//setStart();
		//cout << now_x << "," << now_y << endl;
		//Point other_now(now_x,now_y);
		//ellipse(image, other_now, axes, 0, 0, 360, Scalar(200, 200, 100), 3);
		
		if( selection.width > 0 && selection.height > 0 && first==true){
			trackObject = -1;
			first=false;
		}
            
		if(wIT != way.end()){
			//wIT++;
			next_x = (*wIT)[0];
			next_y = (*wIT)[1];
		}
		Point other(next_x,next_y);
		
		//cout << next_x << "," << next_y << endl;
		//sleep(1);
		ellipse(image, other, axes, 0, 0, 360, Scalar(200, 200, 100), 3);
		Mat contours;

		if( !paused )
        {
			
			 cvtColor(image, hsv, COLOR_BGR2HSV);

			 if( trackObject )
            {
				//trace하는 물체가 있다면
				
                int _vmin = vmin, _vmax = vmax;

                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);
				time (&end);
				
				
				if(difftime (end,start) > settime && time_s==true){
					cout<< "go straght"<<endl;
					system("./go.sh 3");
					time_s = false;
				}
				
                if( trackObject < 0 )
                {
					//잡고나면 나옴.. 한번.
					//cout<< "go straght"<<endl;
					//sleep(5);
					//system("./go.sh 3");
                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                    normalize(hist, hist, 0, 255, CV_MINMAX);

                    trackWindow = selection;
                    trackObject = 1;

                    histimg = Scalar::all(0);
                    int binW = histimg.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, CV_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                        rectangle( histimg, Point(i*binW,histimg.rows),
                                   Point((i+1)*binW,histimg.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }
				//중심 잡기.
                calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
				backproj &= mask;
				RotatedRect trackBox;
				
				int error = 0;
				try{
				   trackBox = CamShift(backproj, trackWindow,
					  
					  TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
				   
				   //trackBoxHistory=trackBox;
				}catch(cv::Exception & e){
					error = 1;
					sleep(2);
					//trackBox=trackBoxHistory;
					trackObject = 0;
					histimg = Scalar::all(0);
				}
				
				//가상의선 y = (y2-y1)/(x2-x1)x+ y1-x1(y2-y1)/(x2-x1)
				//즉 trackBox.center.y = trackBox.center.x*dy/dx + now_y -now_x*dy/dx
				//다음 점에 도착.
				int dx = next_x - now_x;
				int dy = next_y - now_y;
				int lineear;
				int range=20;
				if(dx < 0) dx = dx*(-1);
				if(dy < 0) dy = dy*(-1);
				if(difftime (end,start) > settime){
					if(dx == 0 || dx < dy){
						//cout << trackBox.center.x << "      ,       " <<  now_x << endl;
						line(image, Point(now_x+range,now_y), Point(next_x+range,next_y), Scalar(0), 3);
						line(image, Point(now_x-range,now_y), Point(next_x-range,next_y), Scalar(0), 3);
						
						dx = 1;
						lineear=1;
						if(trackBox.center.x > now_x +range){
							if(now_y > next_y){
								if(lock==false){
								cout<< "1 go left"<<endl;
								system("./go.sh 6");
								lock = true;
								}
							}
							else if(now_y <= next_y){
									if(lock==false){
									cout<< "2 go right"<<endl;
									system("./go.sh 5");
									lock = true;
								}
							}else{
							//락 풀기
								if(lock==true){
									lock = false;
								}
							}
						}else if(trackBox.center.x < now_x-range){
							if(now_y > next_y){
									if(lock==false){
										cout<< "4 go left"<<endl;
									system("./go.sh 6");
									
									lock = true;
								}
							}
							else if(now_y <= next_y){
								if(lock==false){
									cout<< "3 go right"<<endl;
									system("./go.sh 5");
									lock = true;
								}
							}else{
								//락 풀기
								if(lock==true){
									lock = false;
								}
							}
						}
						
					}else{
						line(image, Point(now_x,now_y-range), Point(next_x,next_y-range), Scalar(0), 3);
						line(image, Point(now_x,now_y+range), Point(next_x,next_y+range), Scalar(0), 3);
						
						dx = next_x - now_x;
						lineear = dy/dx;
						
						if(trackBox.center.y > trackBox.center.x*lineear + now_y -now_x*lineear +range){
							//락 확인
							//cout << trackBox.center.y << "  " << trackBox.center.x*lineear + now_y -now_x*lineear +range << endl;
							if(lock==false){
								cout<< "5 go left"<<endl;
								system("./go.sh 6");
								lock = true;
							}
						}else if(trackBox.center.y < trackBox.center.x*lineear + now_y -now_x*lineear -range){
							//락 확인
							if(lock==false){
								cout<< "6 go right"<<endl;
								system("./go.sh 5");
								lock = true;
							}
						}else{
							//락 풀기
							if(lock==true){
								lock = false;
							}
						}
					
					}	
				}					
                if( trackWindow.area() <= 1 )
                {
					cout<< "aaa"<< endl;
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                                  Rect(0, 0, cols, rows);
                }

                if( backprojMode )
                    cvtColor( backproj, image, COLOR_GRAY2BGR );
				
                ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
				
				//C++: void ellipse(Mat& img, Point center, Size axes, double angle, double startAngle, double endAngle, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
				//void ellipse(Mat& img, const RotatedRect& box, const Scalar& color, int thickness=1, int lineType=8)
				if(trackBox.center.y > next_y-50 && trackBox.center.y < next_y+50 && trackBox.center.x > next_x-50 && trackBox.center.x < next_x+50){
					//now를 다음으로 바꿔줌.
					
					
					// 현재 벡터
					int now_vx = next_x - now_x;
					int now_vy = next_y - now_y;
					
					now_x = next_x;
					now_y = next_y;
					if(wIT != way.end()){
						wIT++;
						int new_vx = (*wIT)[0] - now_x;
						int new_vy = (*wIT)[1] - now_y;
						
						//double cos = (now_vx * new_vx + now_vy * new_vy) ;
						double cos = now_vy * new_vx - new_vy * now_vx;
						/// abs(sqrt(pow(now_vx, 2) + pow(now_vy, 2))) / abs(sqrt(pow(new_vx, 2) + pow(new_vy, 2)));
						if(cos > 0){
							// 왼쪽
							cout<<"turn left" << endl;
							system("./go.sh 2");
 	
							//lock = true;
						}else{
							cout<<"turn right" << endl;
							system("./go.sh 1");

							//lock = true;
						}
						
					}else{
						cout<< "stop"<< endl;
						// trackObject = 0;
						// histimg = Scalar::all(0);
						system("./go.sh 0");
						sleep(0.5);
					}
					
				}
            }

		}
        else if( trackObject < 0 )
            paused = false;

        if( selectObject && selection.width > 0 && selection.height > 0 )
        {
			//cout<< "aaa"<< endl; //마우스 클릭할때부터 마우스 클릭 끝날때까지 잡음.
            Mat roi(image, selection);
            bitwise_not(roi, roi);
        }

        imshow( "CamShift Demo", image );
        imshow( "Histogram", histimg );
		
		char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;
        case 'c':
            trackObject = 0;
            histimg = Scalar::all(0);
            break;
        case 'h':
            showHist = !showHist;
            if( !showHist )
                destroyWindow( "Histogram" );
            else
                namedWindow( "Histogram", 1 );
            break;
        case 'p':
            paused = !paused;
            break;
        default:
            ;
        }
   }
   return 0;
}
