

void defaultBoneFunction(inout vector pMat[4])
{
	// swap yz
	vector 	x 	= pMat[0];
	vector 	y 	= pMat[2];
	vector 	z 	= pMat[1];
	vector 	pos = pMat[3];

	pMat[0] = x;
	pMat[1] = y;
	pMat[2] = z;
	pMat[3] = pos * 0.01;
}

vector swapVecMax(inout vector p)
{
	vector ret;

	ret[0]  = p[0];
	ret[1]  = p[2];
	ret[2]  = p[1];

	return ret;
}

void defaultBoneFunctionMax(inout vector pMat[4])
{
	// swap yz
	vector 	x 	= pMat[0];
	vector 	y 	= pMat[2];
	vector 	z 	= pMat[1];
	vector 	pos = pMat[3];

	vector  pos2 = swapVecMax(pos);
	vector 	x2 	= swapVecMax(x);
	vector 	y2 	= swapVecMax(y);
	vector 	z2 	= swapVecMax(z);
	pMat[0] = x2;
	pMat[1] = y2;
	pMat[2] = z2;
	pMat[3] = pos2 * 0.001;
}

void generateSceneRootMax (inout vector pMat[4], int pFrame, int pMaxFrame)
{
	// leave identity

	pMat[0] = "1 0 0";
	pMat[1] = "0 0 1";
	pMat[2] = "0 1 0";
	pMat[3] = "0 0 0";
}

vector swapVec(inout vector p)
{
	vector ret;

	ret[0]  = p[0] * -1.0;
	ret[1]  = p[1] * 1.0;
	ret[2]  = p[2] * -1.0;

	return ret;
}

void rotatePelvis(inout vector pMat[4])
{
	// rotate 180 around y of scene object root matrix
	vector 	x 	= pMat[0];
	vector 	y 	= pMat[1];
	vector 	z 	= pMat[2];
	vector 	pos = pMat[3];

	pMat[0] = swapVec(x);
	pMat[1] = swapVec(y);
	pMat[2] = swapVec(z);
	pMat[3] = swapVec(pos);
}


void invertXPosEntityPos(inout vector pMat[4])
{
	vector 	pos = pMat[3];

	pos[2] = pos[2] * -1.0;

	pMat[3] = swapVec(pos);
}



void generateSceneRoot (inout vector pMat[4], int pFrame, int pMaxFrame)
{
	// leave identity

	pMat[0] = "-1 0 0";
	pMat[1] = "0 0 1";
	pMat[2] = "0 1 0";
	pMat[3] = "0 0 0";
}
