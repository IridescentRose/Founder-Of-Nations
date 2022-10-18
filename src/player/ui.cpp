#include "ui.hpp"


UI::UI(Entity& e) : entity(e), slot_sel(0) {
	texID = Rendering::TextureManager::get().load_texture("./assets/gui.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
	fontID = Rendering::TextureManager::get().load_texture("./assets/default.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);
	itemID = Rendering::TextureManager::get().load_texture("./assets/items.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true, false, true);

	background_bar = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {100, 20} },
		Rendering::Rectangle{ {0, 0}, {50.0f / 128.0f, 10.0f / 128.0f} });

	item_slot = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {24, 24} },
		Rendering::Rectangle{ {50.0f / 128.0f, 0}, {16.0f / 128.0f, 16.0f / 128.0f} });
	item_slot_sel = create_scopeptr<Graphics::G2D::Sprite>(texID,
		Rendering::Rectangle{ {0, 0}, {24, 24} },
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

	for(int i = 0; i < 64; i++) {
		items[i] = create_scopeptr<Graphics::G2D::Sprite>(itemID,
		Rendering::Rectangle{ {0, 0}, {16, 16} },
		Rendering::Rectangle{ {0.125f * static_cast<float>(i % 8), 0.125f * static_cast<float>((i / 8) + 1) }, {16.0f / 128.0f, -16.0f / 128.0f} });
	}
}

void UI::draw(RefPtr<Inventory> inv, bool inInv) {
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

	for (int i = 0; i < 6; i++) {
		item_slot->set_position({ 168 + 24 * i, 0 });
		item_slot->set_layer(-2);
		item_slot->draw();

		auto slot = inv->get_slot(i);
		if(slot.count > 0 && slot.itemID != Item::None) {
			auto id = slot.itemID - 1;

			items[id]->set_position({ 168 + 24 * i + 4, 4});
			items[id]->set_layer(-3);
			items[id]->draw();

			if(slot.count > 1){
				auto str = std::to_string((int)slot.count);
				auto size = font_renderer->calculate_size(str);
				font_renderer->add_text(str, glm::vec2(168 + 24 * i + 23 - size, 1), Rendering::Color{255, 255, 255, 255}, -4);
			}
		} 
	}

	if (inInv) {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 6; x++) {
				item_slot->set_position({ 168 + 24 * x, 40 + y * 32 });
				item_slot->set_layer(-2);
				item_slot->draw();

				auto idx = 6 + x + y * 6;
				auto slot = inv->get_slot(idx);
				if (slot.count > 0 && slot.itemID != Item::None) {
					auto id = slot.itemID - 1;

					items[id]->set_position({ 168 + 24 * x + 4, 40 + y * 32 + 4});
					items[id]->set_layer(-3);
					items[id]->draw();

					if (slot.count > 1) {
						auto str = std::to_string((int)slot.count);
						auto size = font_renderer->calculate_size(str);
						font_renderer->add_text(str, glm::vec2(168 + 24 * x + 23 - size, 1 + 40 + y * 32), Rendering::Color{ 255, 255, 255, 255 }, -4);
					}
				}
			}
		}
	}

	item_slot_sel->set_position({ 168 + 24 * slot_sel, 0 });
	item_slot_sel->set_layer(-4);
	item_slot_sel->draw();

	auto str = "POS: " + std::to_string((int)entity.pos.x) + " " + std::to_string((int)entity.pos.z);
	auto size = font_renderer->calculate_size(str);

	font_renderer->add_text(str, glm::vec2(480 - size, 262), Rendering::Color{ 255, 255, 255, 255 }, -4);

	font_renderer->rebuild();
	font_renderer->draw();

	glEnable(GL_DEPTH_TEST);
}