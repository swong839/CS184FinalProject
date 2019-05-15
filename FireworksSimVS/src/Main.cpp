#include "Testing.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"
#include "ParticleEmitter.h"
#include "Firework.h"
#include <vector>
#include <nanogui/nanogui.h>
#include <iostream>

using namespace nanogui;
using namespace std;

glm::vec3 currentExplosionColor(1, 0, 0);
bool changeExplosionColor = false;
GLfloat currentExplosionForce = 25;
bool changeExplosionForce = false;

GLfloat windVariable = -10;
GLfloat gravityVariable = -9.8;
GLfloat trailLifeVariable = 0.3;
GLfloat explosionLifeVariable = 0.1;

bool randomColor = false;


Firework * CreateFirework();
void RespawnFirework(Firework *firework, const bool stopped = false);

Screen *screen = new Screen();


namespace mySpace
{
		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processInput(GLFWwindow *window, vector<Firework*> &f);
		void switchBetweenWireframe(GLFWwindow *window);
		void restartParticleSystem(GLFWwindow *window, vector<Firework*> &f);
	
		// settings
		const unsigned int SCR_WIDTH = 1920;
		const unsigned int SCR_HEIGHT = 1080;
	
		const string vertexShaderPath = "../../Shaders/Particle.vert";
		const string fragmentShaderPath = "../../Shaders/Particle.frag";
	
		bool isFilling = true;
		bool keyPressed_c = false;
		bool keyPressed_r = false;
	
		// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
		// ---------------------------------------------------------------------------------------------------------
		void processInput(GLFWwindow *window, vector<Firework*> &f)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			switchBetweenWireframe(window);
			restartParticleSystem(window, f);
		}
	
		void switchBetweenWireframe(GLFWwindow *window)
		{
			if (!keyPressed_c && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			{
				keyPressed_c = true;
				if (isFilling)
				{
					isFilling = false;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else
				{
					isFilling = true;
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
			{
				keyPressed_c = false;
			}
		}
	
		void restartParticleSystem(GLFWwindow *window, vector<Firework*> &f)
		{
			if (!keyPressed_r && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			{
				keyPressed_r = true;

				for (Firework * firework : f)
					RespawnFirework(firework);
			}
			else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
			{
				keyPressed_r = false;
			}
		}
	
		// glfw: whenever the window size changed (by OS or user resize) this callback function executes
		// ---------------------------------------------------------------------------------------------
		void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			glViewport(0, 0, width, height);
		}

		void guiInit(Screen *screen) {
			FormHelper *gui = new FormHelper(screen);
			Color color;

			Window *window;

			window = new Window(screen, "Fireworks Simulation");
			window->setPosition(Vector2i(100, 15));
			window->setLayout(new GroupLayout(15, 6, 14, 5));

			/*new Label(window, "Spring types", "sans-bold");
			{
				Button *b = new Button(window, "structural");
				b->setFlags(Button::ToggleButton);
				b->setPushed(true);
				b->setFontSize(14);
				b->setChangeCallback(
					[b](bool state) { cout << "hello" << endl; });
			}*/


			//window = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
			new Label(window, "Parameters", "sans-bold");
			{
				/*Widget *panel = new Widget(window);
				GridLayout *layout =
					new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 5, 5);
				layout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
				layout->setSpacing(0, 10);
				panel->setLayout(layout);

				gui->addWidget("Parameters", panel);*/

				//UI for Gravity Modifier
				new Label(window, "Gravity:", "sans-bold");

				FloatBox<double> *gravity = new FloatBox<double>(window);
				gravity->setEditable(true);
				gravity->setFixedSize(Vector2i(100, 20));
				gravity->setFontSize(14);
				gravity->setValue(gravityVariable);
				gravity->setSpinnable(true);
				gravity->setCallback([gravity](float value) {gravityVariable = value; });

				//UI for WindSpeed
				new Label(window, "WindSpeed toward East:", "sans-bold");

				FloatBox<double> *wind = new FloatBox<double>(window);
				wind->setEditable(true);
				wind->setFixedSize(Vector2i(100, 20));
				wind->setFontSize(14);
				wind->setValue(windVariable);
				wind->setSpinnable(true);
				wind->setCallback([wind](float value) {windVariable = value; });

				//UI for Explosion Lifetime
				new Label(window, "Explosion Lifetime:", "sans-bold");

				FloatBox<double> *exp = new FloatBox<double>(window);
				exp->setEditable(true);
				exp->setFixedSize(Vector2i(100, 20));
				exp->setFontSize(14);
				exp->setValue(explosionLifeVariable);
				exp->setSpinnable(true);
				exp->setCallback([exp](float value) {explosionLifeVariable = value; });

				//UI for Trail Lifetime
				new Label(window, "Trail Lifetime:", "sans-bold");

				FloatBox<double> *trail = new FloatBox<double>(window);
				trail->setEditable(true);
				trail->setFixedSize(Vector2i(100, 20));
				trail->setFontSize(14);
				trail->setValue(trailLifeVariable);
				trail->setSpinnable(true);
				trail->setCallback([trail](float value) {trailLifeVariable = value; });

				//UI for Explosive force
				new Label(window, "Explosion Force:", "sans-bold");

				FloatBox<double> *expForce = new FloatBox<double>(window);
				expForce->setEditable(true);
				expForce->setFixedSize(Vector2i(100, 20));
				expForce->setFontSize(14);
				expForce->setValue(currentExplosionForce);
				expForce->setSpinnable(true);
				expForce->setCallback([expForce](float value) {currentExplosionForce = value; changeExplosionForce = true;  });

				//UI for Red
				new Label(window, "Explosion Color (red):", "sans-bold");

				FloatBox<double> *expRed = new FloatBox<double>(window);
				expRed->setEditable(true);
				expRed->setFixedSize(Vector2i(100, 20));
				expRed->setFontSize(14);
				expRed->setValue(currentExplosionColor.r);
				expRed->setSpinnable(true);
				expRed->setCallback([expRed](float value) {currentExplosionColor.r = value;  });

				//UI for Green
				new Label(window, "Explosion Color (green):", "sans-bold");

				FloatBox<double> *expBlue = new FloatBox<double>(window);
				expBlue->setEditable(true);
				expBlue->setFixedSize(Vector2i(100, 20));
				expBlue->setFontSize(14);
				expBlue->setValue(currentExplosionColor.g);
				expBlue->setSpinnable(true);
				expBlue->setCallback([expBlue](float value) {currentExplosionColor.g = value;  });

				//UI for Blue
				new Label(window, "Explosion Color (blue):", "sans-bold");

				FloatBox<double> *expGreen = new FloatBox<double>(window);
				expGreen->setEditable(true);
				expGreen->setFixedSize(Vector2i(100, 20));
				expGreen->setFontSize(14);
				expGreen->setValue(currentExplosionColor.b);
				expGreen->setSpinnable(true);
				expGreen->setCallback([expGreen](float value) {currentExplosionColor.b = value;  });

				//UI for RandomColor
				new Label(window, "Random Explosion Color:", "sans-bold");

				Button *b = new Button(window, "Random Explosion Color");
				b->setFlags(Button::ToggleButton);
				b->setPushed(randomColor);
				b->setFontSize(14);
				b->setChangeCallback([b](bool state) {randomColor = state; });

				////UI for Color
				//new Label(window, "Firework Color", "sans-bold");
				//ColorWheel *cw = new ColorWheel(window, color);
				//Color *col = new Color(currentExplosionColor.x, currentExplosionColor.y, currentExplosionColor.z, 1.0);
				//cw->setColor(*col);
				//cw->setCallback([cw](const nanogui::Color &color) { currentExplosionColor = glm::vec3(&color.x, &color.y, &color.z); });

			}
			
		}

		void setGLFWCallbacks(GLFWwindow* window) {
			glfwSetCursorPosCallback(window, [](GLFWwindow *, double x, double y) {
				if (!screen->cursorPosCallbackEvent(x, y)) {
					screen->cursorPosCallbackEvent(x / screen->pixelRatio(),
						y / screen->pixelRatio());
				}
			});

			glfwSetMouseButtonCallback(
				window, [](GLFWwindow *, int button, int action, int modifiers) {
				if (!screen->mouseButtonCallbackEvent(button, action, modifiers) ||
					action == GLFW_RELEASE) {
					screen->mouseButtonCallbackEvent(button, action, modifiers);
				}
			});

			glfwSetKeyCallback(
				window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
				if (!screen->keyCallbackEvent(key, scancode, action, mods)) {
					screen->keyCallbackEvent(key, scancode, action, mods);
				}
			});

			glfwSetCharCallback(window, [](GLFWwindow *, unsigned int codepoint) {
				screen->charCallbackEvent(codepoint);
			});

			glfwSetDropCallback(window,
				[](GLFWwindow *, int count, const char **filenames) {
				screen->dropCallbackEvent(count, filenames);
				screen->dropCallbackEvent(count, filenames);
			});

			glfwSetScrollCallback(window, [](GLFWwindow *, double x, double y) {
				if (!screen->scrollCallbackEvent(x, y)) {
					screen->scrollCallbackEvent(x, y);
				}
			});

			glfwSetFramebufferSizeCallback(window,
				[](GLFWwindow *, int width, int height) {
				screen->resizeCallbackEvent(width, height);
				screen->resizeCallbackEvent(width, height);
			});
		}
	}
	
	Firework * CreateFirework()
	{
		std::string vsp = mySpace::vertexShaderPath;
		std::string fsp = mySpace::fragmentShaderPath;
		GLfloat gravity = gravityVariable;
		glm::vec3 windForces(windVariable, 0, 0);
		ExplosionType et = DEFAULT_ET;
		TrailType tt = DEFAULT_TT;
		SmokeType st = DEFAULT_ST;
		Firework *f = new Firework(gravity, windForces, et, vsp, fsp, explosionLifeVariable, tt, vsp, fsp, trailLifeVariable, st, vsp, fsp);
		f->ConfigureShaders(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT);
		return f;
	}


	void RespawnFirework(Firework *firework, const bool stopped)
	{
		glm::vec3 origin = randomVec3(glm::vec3(-100.0f, -80.0f, -180.0f), glm::vec3(100.0f, -70.0f, -170.0f));
		glm::vec3 initialVelocity = randomVec3(glm::vec3(-10.0f, 45.0f, -10.0f), glm::vec3(10.0f, 75.0f, 10.0f));

		if (!stopped)
			firework->Stop();

		GLfloat gravity = gravityVariable;
		glm::vec3 windForces(windVariable, 0, 0);

		firework->Start(origin, initialVelocity,
			currentExplosionColor, changeExplosionColor,
			currentExplosionForce, changeExplosionForce,
			windForces, gravity, randomColor);

		if (changeExplosionColor)
			changeExplosionColor = false;
		if (changeExplosionForce)
			changeExplosionForce = false;
	}

	
	int main()
	{
		init();

		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
		GLFWwindow* window = glfwCreateWindow(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, mySpace::framebuffer_size_callback);


		screen->initialize(window, true);


		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		//Shader particleShader(mySpace::vertexShaderPath, mySpace::fragmentShaderPath);



		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);




		GLuint numFireworks = 19;
		vector<Firework *> fireworks;
		for (size_t i = 0; i < numFireworks; i++)
			fireworks.push_back(CreateFirework());


		GLfloat deltaTime;
		GLfloat lastFrame = 0;
		GLfloat currentFrame = 0;

		size_t curFireworkInd = 0;
		GLfloat timeToNextFirework = 0;
		GLfloat minTimeToNextFirework = 0.5f;
		GLfloat maxTimeToNextFirework = 1.5f;

		mySpace::guiInit(screen);
		screen->setVisible(true);
		screen->performLayout();
		mySpace::setGLFWCallbacks(window);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// Spawning a firework
			if (timeToNextFirework <= 0)
			{
				RespawnFirework(fireworks[curFireworkInd++]);
				if (curFireworkInd == numFireworks)
					curFireworkInd = 0;
				timeToNextFirework = randomFloat(minTimeToNextFirework, maxTimeToNextFirework);
			}
			else
				timeToNextFirework -= deltaTime;

			// Updating time
			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;


			// input
			// -----
			//mySpace::processInput(window, fireworks);

			// render
			// ------
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Firework *f : fireworks)
			{
				f->Update(deltaTime);
				f->Draw();
			}

			//GUI Update Functions
			screen->drawContents();
			screen->drawWidgets();

			mySpace::processInput(window, fireworks);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		for (Firework *f : fireworks)
			delete f;
		fireworks.clear();


		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}
