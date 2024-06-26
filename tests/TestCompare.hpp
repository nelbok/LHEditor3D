#pragma once

#include <QtTest/QtTest>

#include <fsd/data/Character.hpp>
#include <fsd/data/Link.hpp>
#include <fsd/data/Model.hpp>
#include <fsd/data/Place.hpp>
#include <fsd/data/Project.hpp>

struct TestCompare {
	TestCompare(bool isSame)
		: _isSame{ isSame } {}

	virtual ~TestCompare() = default;

	void testCharacter(fsd::Character* left, fsd::Character* right) {
		testPlacement(left, right);

		QCOMPARE(left->hair(), right->hair());
		QCOMPARE(left->skin(), right->skin());
		QCOMPARE(left->clothes(), right->clothes());
		QCOMPARE(left->height(), right->height());
		QCOMPARE(left->species(), right->species());
		QCOMPARE(left->gender(), right->gender());
	}

	void testEntity(fsd::Entity* left, fsd::Entity* right) {
		if (_isSame) {
			QCOMPARE(left->uuid(), right->uuid());
			QCOMPARE(left->name(), right->name());
		} else {
			QVERIFY(left->uuid() != right->uuid());
			QVERIFY(left->name() + "*" == right->name());
		}
	}

	void testLink(fsd::Link* left, fsd::Link* right) {
		testPlacement(left, right);

		testUuidPointer(left->link(), right->link());
	}

	void testModel(fsd::Model* left, fsd::Model* right) {
		testEntity(left, right);

		QCOMPARE(left->sourcePath(), right->sourcePath());
		QCOMPARE(left->qmlName(), right->qmlName());
		QCOMPARE(left->type(), right->type());
	}

	void testObject(fsd::Object* left, fsd::Object* right) {
		testEntity(left, right);

		testUuidPointer(left->model(), right->model());
	}

	void testPlace(fsd::Place* left, fsd::Place* right) {
		testObject(left, right);
	}

	void testPlacement(fsd::Placement* left, fsd::Placement* right) {
		testObject(left, right);

		QCOMPARE(left->position(), right->position());
		QCOMPARE(left->rotation(), right->rotation());
		testUuidPointer(left->place(), right->place());
	}

	void testProject(fsd::Project* left, fsd::Project* right) {
		testEntity(left, right);

		testUuidPointer(left->defaultPlace(), right->defaultPlace());

		{
			const auto& ll = left->characters();
			const auto& rl = right->characters();
			QCOMPARE(ll.size(), rl.size());
			for (qsizetype i = 0; i < ll.size(); ++i) {
				testCharacter(ll.at(i), rl.at(i));
			}
		}

		{
			const auto& ll = left->links();
			const auto& rl = right->links();
			QCOMPARE(ll.size(), rl.size());
			for (qsizetype i = 0; i < ll.size(); ++i) {
				testLink(ll.at(i), rl.at(i));
			}
		}

		{
			const auto& ll = left->models();
			const auto& rl = right->models();
			QCOMPARE(ll.size(), rl.size());
			for (qsizetype i = 0; i < ll.size(); ++i) {
				testModel(ll.at(i), rl.at(i));
			}
		}

		{
			const auto& ll = left->places();
			const auto& rl = right->places();
			QCOMPARE(ll.size(), rl.size());
			for (qsizetype i = 0; i < ll.size(); ++i) {
				testPlace(ll.at(i), rl.at(i));
			}
		}
	}

	void testUuidPointer(fsd::Entity* left, fsd::Entity* right) {
		bool isLeftValid = left != nullptr;
		bool isRightValid = right != nullptr;
		QCOMPARE(isLeftValid, isRightValid);
		if (isLeftValid && isRightValid)
			QCOMPARE(left->uuid(), right->uuid());
	}

private:
	bool _isSame{ true };
};
