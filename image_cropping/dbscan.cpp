#include "dbscan.h"

std::vector<std::vector<cv::KeyPoint> > DBSCAN_keypoints(std::vector<cv::KeyPoint> *keypoints, float eps, int minPts)
{
	std::vector<std::vector<cv::KeyPoint> > clusters;
	std::vector<bool> clustered;
	std::vector<bool> visited;
	std::vector<int> neighborPts;
	std::vector<int> neighborPts_;
	int c;

	int noKeys = keypoints->size();

	//init clustered and visited
	for(int k = 0; k < noKeys; k++)
	{
		clustered.push_back(false);
		visited.push_back(false);
	}

	//C =0;
	c = 0;
	clusters.push_back(std::vector<cv::KeyPoint>()); //will stay empty?

	//for each unvisted point P in dataset keypoints
	for(int i = 0; i < noKeys; i++)
	{
		if(!visited[i])
		{
			//Mark P as visited
			visited[i] = true;
			neighborPts = regionQuery(keypoints,&keypoints->at(i),eps);
			if(neighborPts.size() < minPts)
			{
				//Mark P as Noise
			}
			else
			{
				clusters.push_back(std::vector<cv::KeyPoint>());
				c++;
				//expand cluster
				// add P to cluster c
				clusters[c].push_back(keypoints->at(i));
				clustered[i] = true;
				//for each point P' in neighborPts
				for(int j = 0; j < neighborPts.size(); j++)
				{
					//if P' is not visited
					if(!visited[neighborPts[j]])
					{
						//Mark P' as visited
						visited[neighborPts[j]] = true;
						neighborPts_ = regionQuery(keypoints,&keypoints->at(neighborPts[j]),eps);
						if(neighborPts_.size() >= minPts)
						{
							neighborPts.insert(neighborPts.end(),neighborPts_.begin(),neighborPts_.end());
						}
					}
					// if P' is not yet a member of any cluster
					// add P' to cluster c
					if(!clustered[neighborPts[j]])
					{
						clusters[c].push_back(keypoints->at(neighborPts[j]));
						clustered[neighborPts[j]] = true;
					}
				}
			}

		}
	}
	return clusters;
}

std::vector<int> regionQuery(std::vector<cv::KeyPoint> *keypoints, cv::KeyPoint *keypoint, float eps)
{
	float dist;
	std::vector<int> retKeys;
	for(int i = 0; i< keypoints->size(); i++)
	{
		dist = sqrt(pow((keypoint->pt.x - keypoints->at(i).pt.x),2)+pow((keypoint->pt.y - keypoints->at(i).pt.y),2));
		if(dist <= eps && dist != 0.0f)
		{
			retKeys.push_back(i);
		}
	}
	return retKeys;
}
