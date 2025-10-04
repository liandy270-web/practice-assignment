#include "block.h"
#include <cmath>
#include <iostream>
//	private:
//		vector<vector<RGBAPixel>> data 

int Block::Height() const {
	/* your code here */
	//return data[0].size();
    return data.size();
}

int Block::Width() const {
	/* your code here */
	return data[0].size();
	//return data.size();
}

void Block::Render(PNG &im, int x) const {
	/* your code here */
	for (int y = 0; y < Height(); y++) {
		for (int dx = 0; dx < Width(); dx++) {
			RGBAPixel *currPix = im.getPixel(x + dx, y);
			*currPix = data[y][dx];
		}
	}
	
}

void Block::Build(PNG &im, int x, int width) {
	/* your code here */
//for (unsigned x = 0; x < image.width(); x++) {
//for (unsigned y = 0; y < image.height(); y++) {
//}
//}

    // take image
    // set height from it
    // set width from int
	data.clear(); // wipe old data part of block
	data.resize(im.height()); // resize vector into the image height
	for (int y = 0; y < Height(); y++) {
		data[y].resize(width); // resize inner vector
		for (int dx = 0; dx < Width(); dx++) {
			RGBAPixel *currPix = im.getPixel(x + dx, y);
			data[y][dx] = *currPix;
		}
	}
}
