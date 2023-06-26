#include "Models.h"
#include "Shader.h"

const int X = 0;
const int Y = 1;
const int Z = 2;

float barycentric(std::vector<glm::vec3>& triangle_points, float* pos);
float pull_height(unsigned int index, Maps* height_map);
void  free_map_data(Maps);
Maps  read_map(std::string file_name);



Terrain::Terrain()
{
}

void Terrain::SetUp()
{
	const std::string MAP = "1";

	// HeightMap
	Maps height_map = read_map("HeightMap_" + MAP + ".png");
	std::vector<glm::vec3> y_values;
	
	Maps blend_map = read_map("BlendMap_" + MAP + ".png");

	// Terrain is written into code - (content not derived from external source)
	Model_Own model = GenTerrain(&height_map, y_values);

	this->model_content = model.model_content;

	//this->model_content = model.model_content;
	this->texture_content = model.texture_content;

	this->Set_Counts(this->model_content);
	this->vertice_values = y_values;
	this->vertices_size[0] = height_map.width;
	this->vertices_size[1] = height_map.height;

	setup_buffers(this->buffers, this->model_content, this->texture_content);
	free_map_data(height_map);
}

float Terrain::find_positions_height(float* camera_pos)
{
	unsigned int x_val = camera_pos[X];
	unsigned int z_val = camera_pos[Z];
	unsigned int index = (x_val) + (z_val * this->vertices_size[1]);
	float position_on_tile = ((camera_pos[X] - x_val)/2) + ((camera_pos[Z] - z_val)/2);
	

	std::vector<glm::vec3> triangle_points;
	// Triangle 1:
	// V1-V1
	// V1 

	// Triangle 2:
	//	  V2
	// V2-V2
	if (position_on_tile <= .5f)
		// Triangle 2  - NOTE: might need to be put in right order maybe if issues
		triangle_points =
		{
			this->vertice_values[index],
			this->vertice_values[index + this->vertices_size[0]],
			this->vertice_values[index + 1]
			
		};
	else
		// Triangle 1 
		triangle_points =
		{
			this->vertice_values[index + 1],
			this->vertice_values[index + this->vertices_size[0]],
			this->vertice_values[index + this->vertices_size[0] + 1]

		};


	return barycentric(triangle_points, camera_pos);
}



/* Barry's Algorithm */
float barycentric(std::vector<glm::vec3>& triangle_points, float* pos)
{
	glm::vec3 p1 = triangle_points[0];
	glm::vec3 p2 = triangle_points[1];
	glm::vec3 p3 = triangle_points[2];
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos[X] - p3.x) + (p3.x - p2.x) * (pos[Z] - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos[X] - p3.x) + (p1.x - p3.x) * (pos[Z] - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


// Terrain
Model_Own GenTerrain(Maps* height_map, std::vector<glm::vec3>& vertices)
{
	
	int index = 0;

	const unsigned int VERTICES_PER_COL = (*height_map).width;
	const unsigned int VERTICES_PER_ROW = (*height_map).height;
	const unsigned int SPACE = 1;


	//std::vector<glm::vec3> vertices;
	Model_Own model;



	float y = 0;
	int real_time_offset = 0;
	std::vector<unsigned int> indices_to_use;

	// Boolean Flags
	int texture_coordinate_left = 0;
	int texture_coordinate_top  = -1;




	// Vertices - (Inputted a row at a time)
	for (unsigned int col = 0; col < (VERTICES_PER_COL); col++)
	{ 
		texture_coordinate_left = ~texture_coordinate_left;

	
		for (unsigned int row = 0; row < (VERTICES_PER_ROW); row++)
		{			
			// FROM HEIGHT MAP
			float y_ = pull_height(index, height_map) * 100;
			index++;


			texture_coordinate_top = ~texture_coordinate_top;
			

	
			if (!texture_coordinate_left)
			{
				if (!texture_coordinate_top)
				{
					model.model_content.texture_Coordinates.push_back(1);
					model.model_content.texture_Coordinates.push_back(1);
				}
				else
				{
					model.model_content.texture_Coordinates.push_back(1);
					model.model_content.texture_Coordinates.push_back(0);
				}
			}
			else if (texture_coordinate_left)
			{
				if (!texture_coordinate_top)
				{
					model.model_content.texture_Coordinates.push_back(0);
					model.model_content.texture_Coordinates.push_back(1);
				}
				else
				{
					model.model_content.texture_Coordinates.push_back(0);
					model.model_content.texture_Coordinates.push_back(0);
				}
			}
			



			glm::vec3 vertice = glm::vec3(
				row * SPACE,
				y_,
				col * SPACE
			);
			vertices.push_back(vertice);
		}

	}

	// Triangle 1:
	// V1-V1
	// V1 

	// Triangle 2:
	//	  V2
	// V2-V2
	
	// Indices - (FACES) - (TWO TRIANGLES)
	for (unsigned int col = 0; col < (VERTICES_PER_COL-1); col++)
	{
		int next_column_step = (VERTICES_PER_ROW);
		int starting_index = (VERTICES_PER_ROW*col);

		for (unsigned int row = 0; row < (VERTICES_PER_ROW-1); row++)
		{
			
			unsigned int triangle_1[]
			{
				((starting_index + row) + next_column_step),		// BOTTOM LEFT
				((starting_index + row) + next_column_step + 1),	// TOP LEFT
				(starting_index  + row) + 1							// TOP RIGHT
			};

			
			unsigned int triangle_2[]
			{
			   ((starting_index + row) + next_column_step),			// BOTTOM LEFT
				(starting_index + row) + 1,							// TOP RIGHT
				(starting_index + row)								// BOTTOM RIGHT
			};


			for (unsigned int& index : triangle_1)
				model.model_content.indices.push_back(index);

			for (unsigned int& index : triangle_2)
				model.model_content.indices.push_back(index);

		}
	}
	 

	for (glm::vec3& vec : vertices)
	{
		model.model_content.vertices.push_back(vec.x);
		model.model_content.vertices.push_back(vec.y);
		model.model_content.vertices.push_back(vec.z);
	}

	int uv_count = vertices.size();
	
	model.texture_content.file_names =
	{
		// Textures
		TEXTURE_FOLDER + "Grass.png",
		TEXTURE_FOLDER + "Dirt.png",
		// Maps
		MAPS_FOLDER	   + "BlendMap_1.png",
	};

	return model;
}




/* Height & blend Maps */
Maps read_map(std::string file_name)
{
	Maps height_map;

	int& width			= height_map.width;
	int& height			= height_map.height;
	int& bit_per_pixel	= height_map.bit_per_pixel;

	height_map.data = stbi_load((MAPS_FOLDER + file_name).c_str(), &width, &height, &bit_per_pixel, 0);

	return height_map;
}
float pull_height(unsigned int index, Maps* height_map)
{
	
	unsigned int starting_index = index * 3;
	float val = (float)((int)height_map->data[starting_index] + (int)height_map->data[starting_index + 1] + (int)height_map->data[starting_index + 2]) / MAX_RGB_VAL;
	
	//std::cout << (int)height_map->data[starting_index] << ","
    // 			<< (int)height_map->data[starting_index +1]	<< ","
    // 			<< (int)height_map->data[starting_index +2]	<< " - Y VAL: "
    // 			<< val << std::endl;

	return val;
};
void free_map_data(Maps map)
{	
	stbi_image_free(map.data);
}



void Terrain::ActivateShader(bool activate)
{
	
}