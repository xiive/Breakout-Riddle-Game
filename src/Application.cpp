#include "Application.hpp"
#include "IntroState.hpp"

void Application::run()
{
	// Create render window
	m_window.create({1250, 780}, "Breakout Riddle Game", sf::Style::Titlebar | sf::Style::Close);
	m_window.setFramerateLimit(60);
    
	// Initialize the engine
	m_machine.run(StateMachine::build<IntroState>(m_machine, m_window, true));

	// Main loop
	while (m_machine.running())
	{
		m_machine.nextState();
		m_machine.update();
		m_machine.draw();
	}

	// Leaving the scope of 'Application' will cleanup the engine
}


