#include "Meshes.h"




const enum class FILE_TYPES
{
	OBJ=1, X=2, MTL=3
};




// Count reccurance of spaces in string
unsigned int str_find_val(std::string string, const char* delimiter)
{
	unsigned int count = 1;
	for (unsigned int i = 0; (i = string.find(delimiter, i)) != std::string::npos; i++)
	{
		count++;
	}
	return count;
}


// Reading OBJ
// Splits & Returns contents from prevailing line - (Vertices, Texture-coords & Normals)
std::vector<float> OBJ_append_content(std::string line)
{
	
	std::string line_ = line;
	std::vector<float> list;
	

	// Will always be atleast 1 point. If finds separation " " than increments count 
	unsigned int point_count = str_find_val(line, " ");

	std::string point;
	for (unsigned int i = 0; i < point_count; i++)
	{
		line = line_;
		int space_pos = line.find(" ");
	
		if (space_pos != -1)
			point = line.erase(line.find(" "));
		else
			point = line;
		
		
		// Cast value to float & push back into list
		list.push_back(std::stof(point));
	


		line_.erase(0, space_pos+1);
	}
		
	return list;

	//list.push_back();
	
};
OBJ_Face_3D OBJ_append_indices(std::string line)
{
	line += " ";
	unsigned int faces_on_line = str_find_val(line, " ");
	
	std::vector<std::string> faces;

	OBJ_Face_3D face_content;

	

	unsigned int pos = 0;
	// One face/line will contain THREE vertices. each vertice data will consist of: vert_pos /  vert_uv / vert_norm
	while ((pos = line.find(" ")) != std::string::npos)
	{
		// I added "/" to the end to fix issue with not collecting final value in face
 		std::string face = line.substr(0, pos) + "/";
			
		// Taking first value in face - (vertex position)
		std::string val = face.substr(0, face.find("/"));
		
		

		// Iterate through vertex pos, uv, norm
		int data_position_counter = 0;

		// 3 Points for Triangle 
		unsigned int i;
		while ((i = face.find("/")) != std::string::npos)
		{

			std::string val = face.substr(0, i);
			
			// Blender inserts values starting at 1
			unsigned int data = std::stoi(val)-1;
			
			if (data_position_counter == 0)
				face_content.vertex_pos.push_back(data);

			else if (data_position_counter == 1)
				face_content.vertex_uv.push_back(data);
			
			else
				face_content.vertex_norm.push_back(data);



			data_position_counter++;
			face.erase(0, i+1);
		}


		line.erase(0, pos + 1);
	}

	
	return face_content;

}


/* Reads an OBJ & extract contents */
RawModelContent read_OBJ(std::string file_name)
{


	// NOTE: READ VERTICES, TEX, NORMALS TO LIST OF GLM::VEC3 - ( VEC2 FOR TEX_COORDS) THAN PUT IN ORDER WITH INDICES


	// Will look for: vertices, normals, uvs, indices
	std::ifstream file(file_name);
	std::string current_line;

	RawModelContent model_content;


	std::vector<glm::vec3>		vertex_positions;
	std::vector<glm::vec2>		vertex_tex_coords;
	std::vector<glm::vec3>		vertex_normals;
	std::vector<unsigned int>	indices;


	std::string line;
	while (std::getline(file, current_line))
	{
		// EXTRACTING DATA FROM FILE

		//std::cout << current_line << std::endl;
		line = current_line;

		unsigned int first_space = current_line.find(" ");
		auto line_prefix = current_line.erase(first_space);
		line.erase(0, first_space + 1);

		
		if (line_prefix == "v")
		{	
			std::vector<float> list = OBJ_append_content(line);
			glm::vec3 vertex = glm::vec3(list[0], list[1], list[2]);
			vertex_positions.push_back(vertex);

			// Passing in vertices RAW
			for (int i = 0; i < 3; i++)
			{
				//model_content.vertices.push_back(list[i]);
			}
		}

		else if (line_prefix == "vt")
		{
			std::vector<float> list = OBJ_append_content(line);
			// Have to do 1 - uv.y as opengl uv.y position is flipped
			glm::vec2 uvs = glm::vec2(list[0], list[1]);
			vertex_tex_coords.push_back(uvs);
			
			//std::cout << "Lines UV: " << list[0] << ", " << list[1] << "\n";
			//std::cout << "fixed Y pos: " << 1.f - (float)list[1] << "\n";

			//model_content.texture_Coordinates.push_back(list[0]);
			//model_content.texture_Coordinates.push_back(list[1]);
		}

		else if (line_prefix == "vn")
		{
			std::vector<float> list = OBJ_append_content(line);
			glm::vec3 normals = glm::vec3(list[0], list[1], list[2]);
			vertex_normals.push_back(normals);
		}

		else if (line_prefix == "f")
		{
			/*  --=-= FACE EXAMPLE =-=--
				f 2/1/1 3/2/1 1/3/1     # One Face - (3 Vertices)

				2 / 1 / 1  ==  VertexPosition(v) / TextureCoordinate(vt) / VertexNormal(vn)
				Note: first element is indices to VertexPositions. These are already in order
				although, it is necessary to re-organise the uv's & normals
			*/
			
			// Currently NOT organising UV's to index order
			
			OBJ_Face_3D face = OBJ_append_indices(line);
			

			/*
				ISSUE RESOLVED - (KINDA) :
				Using indices was creating an offset for texture coordinates to some degree (messing up the image)
				Currently not using indices.
				For optimisation learn using indices for texture coordinates
			*/ 
			

			// Organising data with face
			for (int i = 0; i < 3; i++)
			{
				// Vertex Position - (Index)
				model_content.indices.push_back(face.vertex_pos[i]);
				
				glm::vec3 face_vertice_pos = vertex_positions[face.vertex_pos[i]];
				model_content.vertices.push_back(face_vertice_pos.x);
				model_content.vertices.push_back(face_vertice_pos.y);
				model_content.vertices.push_back(face_vertice_pos.z);



				glm::vec2 face_texCoord = vertex_tex_coords[face.vertex_uv[i]];
				model_content.texture_Coordinates.push_back(face_texCoord.x);
				model_content.texture_Coordinates.push_back(1 - face_texCoord.y);
				

				glm::vec3 face_normal = vertex_normals[face.vertex_norm[i]];
				model_content.normals.push_back(face_normal.x);
				model_content.normals.push_back(face_normal.y);
				model_content.normals.push_back(face_normal.z);

			}			

		}

	};


	return model_content;
}


void gen_texture(unsigned int& location, std::string image_file_name, unsigned int texture_location)
{
	glActiveTexture(texture_location);
	glGenTextures(1, &location);
	glBindTexture(GL_TEXTURE_2D, location);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, bit_per_pixel;
	unsigned char* data = stbi_load(image_file_name.c_str(), &width, &height, &bit_per_pixel, 0);

	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bit_per_pixel, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.5f);

		stbi_image_free(data);
		std::cout << "\n[Texture] Successfully loaded: '" << image_file_name << "'";
	}
	else
	{
		std::cout << "\n[Texture] Failed to load: '" << image_file_name << "'";
	}


}






RawTexture read_MTL(std::string file_name)
{

	std::ifstream file(file_name);
	std::string current_line;

	RawTexture texture_content;



	std::string line;
	while (std::getline(file, current_line))
	{
		line = current_line;

		if (current_line.length() != 0)
		{
			unsigned int first_space = current_line.find(" ");
			auto line_prefix = current_line.erase(first_space);
			line.erase(0, first_space + 1);
	

			// Searching for ...:
		


			// File Name
			if (line_prefix == "map_Kd")
			{
				texture_content.file_names.push_back(TEXTURE_FOLDER + line);
			}

		}
	}


	return texture_content;
}




// Init Func - (Collects contents from specified file)
RawModelContent load_OBJ(std::string file, int file_type)
{
	
	std::string file_name = TEXTURE_FOLDER + file;
	RawModelContent model_content;

	if (file_type == (int)FILE_TYPES::OBJ)
	{
		// Reads & returns content from obj file
		model_content = read_OBJ(file_name + ".obj");
	
	}

	return model_content;
}


RawTexture load_MLT(std::string file, int file_type)
{
	std::string file_name = TEXTURE_FOLDER + file;
	RawTexture texture_content = {0};
	if (file_type == (int)FILE_TYPES::MTL)
	{
		texture_content = read_MTL(file_name + ".mtl");
	}

	// Amount of textures determined by checking how many images files are found
	texture_content.textures_count = texture_content.file_names.size();
	return texture_content;
}




// DONE ONCE PER MODEL
// PREDOMINANT VAO MUST BE INITIALLY ACTIVATED BEFORE FUNCTION IS CALLED
/* Will load model data into buffers */
void setup_buffers(Buffers& buffers, RawModelContent& model_content, RawTexture& texture_content)
{
	// References to make code more clear
	// ISSUE NOTE: buffers.<buffers> are not being set up. 
	unsigned int& vbo = buffers.vbo;
	unsigned int& ibo = buffers.ibo;
	unsigned int& tbo = buffers.tbo;
	
	std::vector<float>&				vertices	= model_content.vertices;
	std::vector<unsigned int>&		indices		= model_content.indices;
	std::vector<float>&				uvs			= model_content.texture_Coordinates;



	// Creating Vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	


	// Creating Index Buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
		


	// Creating UVs buffer
 	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_DYNAMIC_DRAW);

	// TEXTURE IS LOADED DURING FILE EXTRACTION ATM
	texture_content.model_textures = (unsigned int*)malloc(sizeof(unsigned int) * texture_content.file_names.size());
	texture_content.textures_count = texture_content.file_names.size();
	for (unsigned int i = 0; i < texture_content.textures_count; i++)
		gen_texture(texture_content.model_textures[i], texture_content.file_names[i], i);
}

// NOTES: Still trying to convert over to using program class. Models need to be set up 
void Model::SetUp(std::string FileName)
{
	RawModelContent model_content;

	model_content = load_OBJ(FileName, (int)FILE_TYPES::OBJ);
	this->texture_content = load_MLT(FileName, (int)FILE_TYPES::MTL);
	                         
	this->model_content = model_content;
	// Fill buffers with content of a model 
	setup_buffers(this->buffers, model_content, this->texture_content);
	this->Set_Counts(model_content);
}

// MUST BE LOADED AT START
Model_Global::Model_Global()
{

}





