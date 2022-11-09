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


void GroupNode::BindShader(Shader* s) {
	currentShader = s;
	glUseProgram(s->GetProgram());
}

void GroupNode::Render() {
	for (auto const& i : children) {
		if (i->GetNodeType() == GEOMETRY) {
			BindShader(dynamic_cast<GeometryNode*>(i)->GetShader());
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, dynamic_cast<GeometryNode*>(i)->GetModelMatrix().values);
			for (auto const& i : children) {
				if (i->GetNodeType() == CAMERA) {
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetViewMatrix().values);
					glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, dynamic_cast<Camera*>(i)->GetProjectionMatrix().values);

				}
			}
			dynamic_cast<GeometryNode*>(i)->Render();
		}
	}
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
