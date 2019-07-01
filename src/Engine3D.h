#pragma once
#include "ResourceManager.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "FastNoise.h"
#include "Renderer.h"
#include "defines.h"
#include "Camera.h"
#include "CloudSystem.h"
#include "Texture3D.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Engine3D{	
	public:
		GameStateManager GSM;
		Engine3D();
		~Engine3D();
		double startTime;
		double const targetUPSTime = 1.0/TARGET_UPS;
		double previousUpdateTime;
		double previousRenderTime;
		int UPSCount = 0;
		int FPSCount = 0;
		int aUPS = 0;
		int aFPS = 0;

		int const GL_CONTEXT_VERSION_MAJOR = 4;
		int const GL_CONTEXT_VERSION_MINOR = 0;
		int const IS_VSYNC_ON = 0;
		GLint WIDTH = 104 * 16, HEIGHT = 104 * 9;
		GLFWwindow *window;

		Renderer renderer;
		std::string currentShader = "yac";
		// std::string currentShader = "cloudscape";
		CloudSystem cloudSystem;
		Camera camera;
		glm::mat4 staticCam;
		// static glm::vec2 lastMousePosition;


		//Imgui vars
		bool showMainMenu = true;
		bool *p_open = &showMainMenu;
    	ImGuiWindowFlags window_flags = 0;

		void initInputManager() {
			glfwSetKeyCallback(window, InputManager::key_callback_handler);
			glfwSetCursorPosCallback(window, InputManager::mouse_callback);

			//Instantiate and init self
			InputManager::getSelf();
		}

		void initCamera() {
			glm::vec3 position(0, 0, 0);
			camera.setPosition(position);
			glm::vec3 front = { 0.0f, 0.0f, 1.0f };
			glm::vec3 up = { 0.0f, 1.0f, 0.0f };
			glm::vec3 side = glm::cross(front, up);
			staticCam = glm::lookAt(position, position + front, up);
		}

		void initImgui(){
			// Setup Dear ImGui binding
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init();

			// Setup style
			ImGui::StyleColorsDark();
		}

		void init() {
			startGLFW();
			initInputManager();
			initCamera();
			cloudSystem.generateCloudNoiseTextures();

			glEnable(GL_TEXTURE_2D);
			glFrontFace(GL_CCW);
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);

			initImgui();
		}

		int startGLFW() {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_CONTEXT_VERSION_MAJOR);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_CONTEXT_VERSION_MINOR);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


			window = glfwCreateWindow(WIDTH, HEIGHT, "Narval Engine - alpha", nullptr, nullptr);
			if (window == nullptr){
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return -1;
			}

			glfwMakeContextCurrent(window);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize GLAD" << std::endl;
				return -1;
			}

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
			glfwSwapInterval(IS_VSYNC_ON);
		}

		void imguiShutdown(){
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void mainLoop() {
			while (!glfwWindowShouldClose(window)) {

				if (glfwGetTime() - startTime >= 1.0) {
					// std::cout << "------------------------" << std::endl;
					// std::cout << "UPS: " << UPSCount << std::endl;
					// std::cout << "FPS: " << FPSCount << std::endl;
					aUPS = UPSCount;
					aFPS = FPSCount;
					startTime = glfwGetTime();
					UPSCount = 0;
					FPSCount = 0;
				}

				update();
				render();
			}
			imguiShutdown();

			// glfw: terminate, clearing all previously allocated GLFW resources.
			glfwDestroyWindow(window);
			glfwTerminate();
			// To-do: It is necessary to delete the data, we do not have a garbage collector here
		}

		void update() {
			if (glfwGetTime() - previousUpdateTime >= targetUPSTime) {
				glfwGetWindowSize(window, &WIDTH, &HEIGHT);
				camera.update();

				previousUpdateTime = glfwGetTime();
				GSM.update();
				UPSCount++;
			}
			glfwPollEvents();
		}

		void renderImGUI(){
			// Show GUI
			// Start the ImGui frame (For Each new frame)
        	ImGui_ImplOpenGL3_NewFrame();
        	ImGui_ImplGlfw_NewFrame();
        	ImGui::NewFrame();
			
			
			if (showMainMenu){
        		ImGui::SetNextWindowPos(ImVec2(1200, 4), ImGuiCond_Once); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        		ImGui::SetNextWindowSize(ImVec2(450, 900), ImGuiCond_Once);
            	ImGui::Begin("Main Menu", p_open, window_flags);
				ImGui::BulletText("UPS: %d", aUPS);
				ImGui::BulletText("FPS: %d", aFPS);
				ImGui::End();
			}
			// Imgui Render Calls (For Each end frame)
        	ImGui::Render();
        	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void render() {
			glViewport(0, 0, WIDTH, HEIGHT);
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 model(1);
			glm::mat4 proj(1);
			model = glm::translate(model, { 0, 0, 5 });
			model = glm::scale(model, { 1, 1, 1 });
			float nearPlane = 1;
			float farPlane = 60000;
			float projAngle = 45;
			proj = glm::perspective(glm::radians(projAngle), (GLfloat)WIDTH / (GLfloat)HEIGHT, nearPlane, farPlane);


			ResourceManager::getSelf()->getShader(currentShader).use();
			//should be done only once
			ResourceManager::getSelf()->getShader(currentShader).setInteger("perlinWorley", 0);
			glActiveTexture(GL_TEXTURE0);
			cloudSystem.perlinWorley3.bind();

			ResourceManager::getSelf()->getShader(currentShader).setInteger("worley3", 1);
			glActiveTexture(GL_TEXTURE1);
			cloudSystem.worley3.bind();

			ResourceManager::getSelf()->getShader(currentShader).setInteger("curl3", 2);
			glActiveTexture(GL_TEXTURE2);
			cloudSystem.curl3.bind();

			ResourceManager::getSelf()->getShader(currentShader).setInteger("iChannel0", 3);
			glActiveTexture(GL_TEXTURE3);
			ResourceManager::getSelf()->getTexture2D("weather").bind();

			float time = ((sin(glm::radians(glfwGetTime()*10)) + 1) / 2)*1;

			glm::vec3 camPos = *(camera.getPosition());

			ResourceManager::getSelf()->getShader(currentShader).setFloat("iTime", glfwGetTime());
			// ResourceManager::getSelf()->getShader(currentShader).setMat4("cam", *camera.getCam());
			//ResourceManager::getSelf()->getShader(currentShader).setMat4("staticCam", staticCam);
			glm::mat4 invCam = glm::inverse(*camera.getCam());
			//ResourceManager::getSelf()->getShader(currentShader).setMat4("inverseCam", invCam);
			// ResourceManager::getSelf()->getShader(currentShader).setMat4("model", model);
			// ResourceManager::getSelf()->getShader(currentShader).setMat4("projection", proj);
			glm::mat4 invProj = glm::inverse(proj);
			//ResourceManager::getSelf()->getShader(currentShader).setMat4("inverseProj", invProj);
			ResourceManager::getSelf()->getShader(currentShader).setVec2("iResolution", WIDTH, HEIGHT);
			// ResourceManager::getSelf()->getShader(currentShader).setVec3("sunlightDirection", 0,1,0);
			// ResourceManager::getSelf()->getShader(currentShader).setVec3("cameraPosition", camPos.x, camPos.y, camPos.z);
			ResourceManager::getSelf()->getShader(currentShader).setVec2("mousePosition", InputManager::getSelf()->getMousePosition().x, InputManager::getSelf()->getMousePosition().y);
			
			renderer.render(ResourceManager::getSelf()->getModel("quadTest"));

			/*model = glm::mat4(1);
			model = glm::translate(model, { 0, 0, 8 });
			model = glm::scale(model, { 1, 1, 1 });
			ResourceManager::getSelf()->getShader(currentShader).setMat4("model", model);
			renderer.render(ResourceManager::getSelf()->getModel("cubeTest"));*/

			previousRenderTime = glfwGetTime();
			GSM.render();
			FPSCount++;
			renderImGUI();
			glfwSwapBuffers(window);
		}
};

