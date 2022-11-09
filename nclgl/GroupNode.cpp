#include "GroupNode.h"
#include "GeometryNode.h"
#include "Camera.h"

GroupNode::~GroupNode() {
	for (const auto& i: children) {
		delete i;
	}
}

void GroupNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->SetParent(this);
}


void GroupNode::Render() {
	//what to do here?? they are not renderable
		//types of nodes
		//get geometry node:
		//bind shader
		//send model matrix matrix to gpu
		// BindShader();
		// glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);

		//now get camera nodes
		//send camera info to gpu (model and proj)

		//for loop dlights
		//send dlights info to gpu in array (bulk)

		//same for other types of lights
}

void GroupNode::Update(float dt) {
	/*for (auto const& i : children) {
		i->Update(dt);
	}*/
	// what to do here?
}
