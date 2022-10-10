#include "ui.hpp"


UI::UI(Entity& e) : entity(e), slot_sel(0) {
	texID = Rendering::TextureManager::get().load_texture("./assets/gui.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
	fontID = Rendering::TextureManager::get().load_texture("./assets/default.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);

	background_bar = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {100, 20} },
		Rendering::Rectangle{ {0, 0}, {50.0f / 128.0f, 10.0f / 128.0f} });

	item_slot = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {32, 32} },
		Rendering::Rectangle{ {50.0f / 128.0f, 0}, {16.0f / 128.0f, 16.0f / 128.0f} });
	item_slot_sel = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {32, 32} },
		Rendering::Rectangle{ {66.0f / 128.0f, 0}, {16.0f / 128.0f, 16.0f / 128.0f} });

	red_bar = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {100, 20} },
		Rendering::Rectangle{ {0, 10.0f / 128.0f}, {50.0f / 128.0f, 10.0f / 128.0f} });
	blue_bar = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {100, 20} },
		Rendering::Rectangle{ {0, 20.0f / 128.0f}, {50.0f / 128.0f, 10.0f / 128.0f} });
	green_bar = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {100, 20} },
		Rendering::Rectangle{ {0, 30.0f / 128.0f}, {50.0f / 128.0f, 10.0f / 128.0f} });

	font_renderer = create_scopeptr<Graphics::G2D::FontRenderer>(fontID, glm::vec2(16, 16));
}

void UI::draw() {
	glDisable(GL_DEPTH_TEST);

	for (int i = 0; i < 3; i++) {
		background_bar->set_position({ 0, 252 - i * 20});
		background_bar->set_layer(-1);
		background_bar->draw();
	}

	font_renderer->clear();

	float percentHP = (float)entity.hp / (float)entity.base_hp;
	red_bar->set_position({ 0, 252 });
	red_bar->set_size({ 100 * percentHP, 20 });
	red_bar->set_layer(-2);
	red_bar->draw();
	font_renderer->add_text("HP: " + std::to_string(entity.hp) + " / " + std::to_string(entity.base_hp), glm::vec2(4, 258), Rendering::Color{ 255, 255, 255, 255 }, -3);

	float percentEnergy = (float)entity.energy / (float)entity.base_energy;
	blue_bar->set_position({ 0, 232 });
	blue_bar->set_size({ 100 * percentEnergy, 20 });
	blue_bar->set_layer(-2);
	blue_bar->draw();
	font_renderer->add_text("ENERGY: " + std::to_string(entity.energy) + " / " + std::to_string(entity.base_energy), glm::vec2(4, 238), Rendering::Color{ 255, 255, 255, 255 }, -3);

	float percentXP = (float)entity.xp / (float)entity.next_xp;
	green_bar->set_position({ 0, 212 });
	green_bar->set_size({ 100 * percentXP, 20 });
	green_bar->set_layer(-2);
	green_bar->draw();
	font_renderer->add_text("XP: " + std::to_string(entity.xp) + " / " + std::to_string(entity.next_xp), glm::vec2(4, 218), Rendering::Color{ 255, 255, 255, 255 }, -3);

	font_renderer->rebuild();
	font_renderer->draw();

	for (int i = 0; i < 6; i++) {
		item_slot->set_position({ 144 + 32 * i, 0 });
		item_slot->set_layer(-2);
		item_slot->draw();
	}

	item_slot_sel->set_position({ 144 + 32 * slot_sel, 0 });
	item_slot_sel->set_layer(-3);
	item_slot_sel->draw();

	glEnable(GL_DEPTH_TEST);
}