

#include <chrono>
 

//Model GenTerrain(unsigned int facesPerRow, unsigned int facesPerColumn)
//{
//	Model model;
//
//	std::vector<glm::vec3> vertices;
//	const float FACESIZE = .2f;
//
//
//	// Texture coordinates to be applied per face
//	const float FACE_TEXTURE_COORDINATES[]
//	{
//		0, 0,
//		0, 1,
//		1, 0,
//		1, 1,
//	};
//
//	float rowSpace = 0;
//	float colSpace = 0;
//	float y = 0;
//
//	for (unsigned int faceCol = 0; faceCol < facesPerColumn; faceCol++)
//	{
//		rowSpace = 0;
//
//		float column = FACESIZE * faceCol + colSpace;
//		colSpace += FACESIZE;
//
//		float row = 0;
//
//
//		// Refresh random values by applying current time - (in nano seconds)
//		srand(
//			(unsigned int)std::chrono::duration_cast<std::chrono::nanoseconds>
//			(std::chrono::system_clock::now().time_since_epoch()).count()
//		);
//
//		for (unsigned int faceRow = 0; faceRow < facesPerRow; faceRow++)
//		{
//			float random_val = (float)(rand() % 10 + 1);
//
//			//y += .1f;
//			float current_y = y + (random_val / 100);
//
//
//			row = (FACESIZE * faceRow) + rowSpace;
//			rowSpace += FACESIZE;
//
//
//			/* ------------------*/
//			vertices.push_back(glm::vec3(column, current_y, row));					// BottomLeft
//			vertices.push_back(glm::vec3(column, current_y, row + FACESIZE));		// TopLeft  						 
//			vertices.push_back(glm::vec3(column + FACESIZE, current_y, row));					// BottomRight
//			vertices.push_back(glm::vec3(column + FACESIZE, current_y, row + FACESIZE));		// TopRight
//																							/* ------------------*/
//
//			for (const float& uv : FACE_TEXTURE_COORDINATES)
//			{
//				model.model_content.texture_Coordinates.push_back(uv);
//			}
//		}
//	}
//
//	/* Indices */
//	unsigned int oneFace = 4;
//	for (unsigned int faceCol = 0; faceCol < facesPerColumn; faceCol++)
//	{
//		for (unsigned int faceRow = 0; faceRow < facesPerRow; faceRow++)
//		{
//			// Each face has 4 vertices 
//			unsigned int index = (faceRow * oneFace) + (faceCol * (facesPerRow * 4));
//
//			model.model_content.indices.push_back(index + 3);
//			model.model_content.indices.push_back(index);
//			model.model_content.indices.push_back(index + 1);
//
//			model.model_content.indices.push_back(index + 3);
//			model.model_content.indices.push_back(index);
//			model.model_content.indices.push_back(index + 2);
//		}
//
//		for (unsigned int faceRow = 1; faceRow < facesPerRow; faceRow++)
//		{
//			// Each face has 4 vertices 
//			unsigned int index = (faceRow * 4) + (faceCol * (facesPerRow * 4));
//
//			model.model_content.indices.push_back(index - 3);
//			model.model_content.indices.push_back(index);
//			model.model_content.indices.push_back(index - 1);
//
//			model.model_content.indices.push_back(index);
//			model.model_content.indices.push_back(index + 2);
//			model.model_content.indices.push_back(index - 1);
//		}
//
//
//
//		if (faceCol < facesPerColumn - 1)
//		{
//			for (unsigned int faceRow = 0; faceRow < facesPerRow; faceRow++)
//			{
//
//				// Each face has 4 vertices 
//				unsigned int index = (faceRow * 4) + (faceCol * (facesPerRow * 4));
//
//
//				model.model_content.indices.push_back(index + 3);
//				model.model_content.indices.push_back(index + 2);
//				model.model_content.indices.push_back(index + (facesPerRow * 4));
//
//				model.model_content.indices.push_back(index + (facesPerRow * 4));
//				model.model_content.indices.push_back(index + 3);
//				model.model_content.indices.push_back(index + 1 + (facesPerRow * 4));
//
//			}
//
//
//			for (unsigned int faceRow = 1; faceRow < facesPerRow; faceRow++)
//			{
//				// Each face has 4 vertices 
//				unsigned int index = (faceRow * 4) + (faceCol * (facesPerRow * 4));
//
//				model.model_content.indices.push_back(index - 3 + (facesPerRow * 4)); //
//				model.model_content.indices.push_back(index + 2);  //
//				model.model_content.indices.push_back(index + (facesPerRow * 4));
//
//				model.model_content.indices.push_back(index - 3 + (facesPerRow * 4)); // leave
//				model.model_content.indices.push_back(index - 1);  // leave
//				model.model_content.indices.push_back(index + 2); // edit
//			}
//		}
//	}
//
//
//
//	/* Raw Vertices */
//	for (unsigned int i = 0; i < vertices.size(); i++)
//	{
//		model.model_content.vertices.push_back(vertices[i].x);
//		model.model_content.vertices.push_back(vertices[i].y);
//		model.model_content.vertices.push_back(vertices[i].z);
//	}
//
//
//	// Normals										- Normal for each vertex
//	std::vector<float> normals;
//	for (unsigned int i = 0; i < vertices.size() - 4; i++)
//	{
//		glm::vec3 cross = glm::cross(glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z),					// Current
//			glm::vec3(vertices[i + 4].x, vertices[i + 4].y, vertices[i + 4].z));								// Neighbour
//
//		glm::vec3 normal = glm::normalize(cross);
//
//		for (int j = 0; j < 3; j++)
//		{
//			model.model_content.normals.push_back(normal[j]);
//		}
//	};
//
//	
//
//	// Texture Content
//	model.texture_content.file_name = TEXTURE_FOLDER + "Dirt.png";
//
//	return model;
//}