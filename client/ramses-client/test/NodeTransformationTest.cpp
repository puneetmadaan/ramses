//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include <gtest/gtest.h>

#include "ramses-client-api/MeshNode.h"
#include "ramses-client-api/PerspectiveCamera.h"
#include "ramses-client-api/OrthographicCamera.h"
#include "ramses-client-api/PickableObject.h"

#include "ClientTestUtils.h"
#include "Math3d/Vector3.h"
#include "RamsesObjectTestTypes.h"
#include "TestEqualHelper.h"

namespace ramses
{
    using namespace testing;

    template <typename T>
    class NodeTransformationTest : public LocalTestClientWithScene, public testing::Test
    {
    protected:
        virtual void SetUp() override
        {
            m_node = &this->template createObject<T>("node");
        }

        T* m_node;
    };

    TYPED_TEST_SUITE(NodeTransformationTest, NodeTypes);

    TYPED_TEST(NodeTransformationTest, setTranslate)
    {
        ramses_internal::Vector3 initialTranslation(0.f, 0.f, 0.f);
        ramses_internal::Vector3 actualTranslation;
        EXPECT_EQ(StatusOK, this->m_node->getTranslation(actualTranslation.x, actualTranslation.y, actualTranslation.z));
        EXPECT_EQ(initialTranslation, actualTranslation);

        ramses_internal::Vector3 translationVector(1.2f, 2.3f, 4.5f);
        EXPECT_EQ(StatusOK, this->m_node->setTranslation(translationVector.x, translationVector.y, translationVector.z));
        EXPECT_EQ(StatusOK, this->m_node->getTranslation(actualTranslation.x, actualTranslation.y, actualTranslation.z));
        EXPECT_EQ(translationVector, actualTranslation);
    }

    TYPED_TEST(NodeTransformationTest, translate)
    {
        ramses_internal::Vector3 initialTranslation(0.f, 0.f, 0.f);
        ramses_internal::Vector3 actualTranslation;
        EXPECT_EQ(StatusOK, this->m_node->getTranslation(actualTranslation.x, actualTranslation.y, actualTranslation.z));
        EXPECT_EQ(initialTranslation, actualTranslation);

        ramses_internal::Vector3 translationVector(1.2f, 2.3f, 4.5f);
        EXPECT_EQ(StatusOK, this->m_node->translate(translationVector.x, translationVector.y, translationVector.z));
        EXPECT_EQ(StatusOK, this->m_node->translate(translationVector.x, translationVector.y, translationVector.z));
        EXPECT_EQ(StatusOK, this->m_node->getTranslation(actualTranslation.x, actualTranslation.y, actualTranslation.z));
        EXPECT_EQ(2 * translationVector, actualTranslation);
    }

    TYPED_TEST(NodeTransformationTest, setRotation)
    {
        ramses_internal::Vector3 initialRotation(0.f, 0.f, 0.f);
        ramses_internal::Vector3 actualRotation;
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(initialRotation, actualRotation);

        ramses_internal::Vector3 rotationVector_1(1.2f, 2.3f, 4.5f);
        EXPECT_EQ(StatusOK, this->m_node->setRotation(rotationVector_1.x, rotationVector_1.y, rotationVector_1.z));
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(rotationVector_1, actualRotation);

        ramses_internal::Vector3 rotationVector_2(2.2f, 3.3f, 5.5f);
        EXPECT_EQ(StatusOK, this->m_node->setRotation(rotationVector_2.x, rotationVector_2.y, rotationVector_2.z));
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(rotationVector_2, actualRotation);
    }

    TYPED_TEST(NodeTransformationTest, rotate)
    {
        ramses_internal::Vector3 initialRotation(0.f, 0.f, 0.f);
        ramses_internal::Vector3 actualRotation;
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(initialRotation, actualRotation);

        ramses_internal::Vector3 rotationVector_1(1.f, 2.f, 3.f);
        EXPECT_EQ(StatusOK, this->m_node->rotate(rotationVector_1.x, rotationVector_1.y, rotationVector_1.z));
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(rotationVector_1, actualRotation);

        ramses_internal::Vector3 rotationVector_2(4.5f, 2.5f, 0.5f);
        EXPECT_EQ(StatusOK, this->m_node->rotate(rotationVector_2.x, rotationVector_2.y, rotationVector_2.z));

        ramses_internal::Vector3 resultVector(5.5f, 4.5f, 3.5f);
        EXPECT_EQ(StatusOK, this->m_node->getRotation(actualRotation.x, actualRotation.y, actualRotation.z));
        EXPECT_EQ(resultVector, actualRotation);
    }

    TYPED_TEST(NodeTransformationTest, setScaling)
    {
        ramses_internal::Vector3 initialScaling(1.f, 1.f, 1.f);
        ramses_internal::Vector3 actualScale;
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(initialScaling, actualScale);

        ramses_internal::Vector3 scalingVector_1(1.2f, 2.3f, 4.5f);
        EXPECT_EQ(StatusOK, this->m_node->setScaling(scalingVector_1.x, scalingVector_1.y, scalingVector_1.z));
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(scalingVector_1, actualScale);

        ramses_internal::Vector3 scalingVector_2(2.2f, 3.3f, 5.5f);
        EXPECT_EQ(StatusOK, this->m_node->setScaling(scalingVector_2.x, scalingVector_2.y, scalingVector_2.z));
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(scalingVector_2, actualScale);
    }

    TYPED_TEST(NodeTransformationTest, scale)
    {
        ramses_internal::Vector3 initialScaling(1.f, 1.f, 1.f);
        ramses_internal::Vector3 actualScale;
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(initialScaling, actualScale);

        ramses_internal::Vector3 scalingVector_1(4.f, 6.f, 8.f);
        EXPECT_EQ(StatusOK, this->m_node->scale(scalingVector_1.x, scalingVector_1.y, scalingVector_1.z));
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(scalingVector_1, actualScale);

        ramses_internal::Vector3 scalingVector_2(0.5f, 0.5f, 0.5f);
        EXPECT_EQ(StatusOK, this->m_node->scale(scalingVector_2.x, scalingVector_2.y, scalingVector_2.z));

        ramses_internal::Vector3 resultVector(2.f, 3.f, 4.f);
        EXPECT_EQ(StatusOK, this->m_node->getScaling(actualScale.x, actualScale.y, actualScale.z));
        EXPECT_EQ(resultVector, actualScale);
    }

    TYPED_TEST(NodeTransformationTest, rotateLegacyZYXAndNonLegacyConventions)
    {
        const ramses_internal::Vector3 rotationAngles(30.f, 60.f, 120.f);
        EXPECT_EQ(StatusOK, this->m_node->setRotation(rotationAngles.x, rotationAngles.y, rotationAngles.z));
        const auto expectedLegacyZYXMatrix = ramses_internal::Matrix44f::RotationEuler(rotationAngles, ramses_internal::ERotationConvention::Legacy_ZYX);

        ramses_internal::Matrix44f resultNodeMatrix;
        this->m_node->getModelMatrix(resultNodeMatrix.data);
        expectMatrixFloatEqual(expectedLegacyZYXMatrix, resultNodeMatrix);

        TypeParam* childNode = &this->template createObject<TypeParam>("child node");
        this->m_node->addChild(*childNode);

        ramses_internal::Matrix44f resultChlidNodeMatrix;
        childNode->getModelMatrix(resultChlidNodeMatrix.data);
        expectMatrixFloatEqual(expectedLegacyZYXMatrix, resultChlidNodeMatrix);

        EXPECT_EQ(StatusOK, childNode->setRotation(rotationAngles.x, rotationAngles.y, rotationAngles.z, ramses::ERotationConvention::XYZ));
        childNode->getModelMatrix(resultChlidNodeMatrix.data);
        expectMatrixFloatEqual(ramses_internal::Matrix44f::Identity, resultChlidNodeMatrix);

        const ramses_internal::Vector3 childRotationAngles(35.f, 65.f, 125.f);
        EXPECT_EQ(StatusOK, childNode->setRotation(childRotationAngles.x, childRotationAngles.y, childRotationAngles.z, ramses::ERotationConvention::XYX));
        childNode->getModelMatrix(resultChlidNodeMatrix.data);
        const auto expectedChildRotateMatrix = expectedLegacyZYXMatrix *
                                                ramses_internal::Matrix44f::RotationEuler(childRotationAngles, ramses_internal::ERotationConvention::XYX);
        expectMatrixFloatEqual(expectedChildRotateMatrix, resultChlidNodeMatrix);
    }

    TYPED_TEST(NodeTransformationTest, rotateMixConventions)
    {
        /*
                     node
                (10, 0, 0, XYZ)
                /              \
             chlid0           child1
         (0, 20, 0, ZYX)    (0, 20, 30, ZYZ)
                |
            grandChild
          (0, 0, 30, YZX)
        */
        TypeParam* child0 = &this->template createObject<TypeParam>("child0 node");
        TypeParam* child1 = &this->template createObject<TypeParam>("child1 node");
        TypeParam* grandChild = &this->template createObject<TypeParam>("grand child node");
        this->m_node->addChild(*child0);
        this->m_node->addChild(*child1);
        child0->addChild(*grandChild);

        EXPECT_EQ(StatusOK, this->m_node    ->setRotation(10.f, 0.f , 0.f , ramses::ERotationConvention::XYZ));
        EXPECT_EQ(StatusOK, child0          ->setRotation(0.f , 20.f, 0.f , ramses::ERotationConvention::ZYX));
        EXPECT_EQ(StatusOK, child1          ->setRotation(0.f , 20.f, 30.f, ramses::ERotationConvention::ZYZ));
        EXPECT_EQ(StatusOK, grandChild      ->setRotation(0.f ,  0.f, 30.f, ramses::ERotationConvention::YZX));

        //expected matrices for rotation after transformation chain is applied
        const auto expectedNodeRotationMatrix           = ramses_internal::Matrix44f::RotationEuler({ 10.f , 0.f , 0.f  }, ramses_internal::ERotationConvention::XYZ);
        const auto expectedChild0RorationMatrix         = ramses_internal::Matrix44f::RotationEuler({ 10.f , 20.f, 0.f  }, ramses_internal::ERotationConvention::XYZ);
        const auto expectedChild1RorationMatrix         = ramses_internal::Matrix44f::RotationEuler({ 10.f , 20.f, 30.f }, ramses_internal::ERotationConvention::XYZ);
        const auto expectedGrandChildRorationMatrix     = ramses_internal::Matrix44f::RotationEuler({ 10.f , 20.f, 30.f }, ramses_internal::ERotationConvention::XYZ);

        ramses_internal::Matrix44f resultNodeMatrix;
        ramses_internal::Matrix44f resultChild0Matrix;
        ramses_internal::Matrix44f resultChild1Matrix;
        ramses_internal::Matrix44f resultGrandChildMatrix;
        this->m_node->getModelMatrix(resultNodeMatrix.data);
        child0      ->getModelMatrix(resultChild0Matrix.data);
        child1      ->getModelMatrix(resultChild1Matrix.data);
        grandChild  ->getModelMatrix(resultGrandChildMatrix.data);

        expectMatrixFloatEqual(expectedNodeRotationMatrix       , resultNodeMatrix);
        expectMatrixFloatEqual(expectedChild0RorationMatrix     , resultChild0Matrix);
        expectMatrixFloatEqual(expectedChild1RorationMatrix     , resultChild1Matrix);
        expectMatrixFloatEqual(expectedGrandChildRorationMatrix , resultGrandChildMatrix);
    }

    template <typename T>
    class NodeTransformationTestWithPublishedScene : public LocalTestClientWithScene, public testing::Test
    {
    protected:
        virtual void SetUp() override
        {
            const ramses_internal::IScene& iscene = this->m_scene.impl.getIScene();
            ramses_internal::SceneInfo info(iscene.getSceneId(), iscene.getName());
            EXPECT_CALL(this->sceneActionsCollector, handleNewSceneAvailable(info, _));
            EXPECT_CALL(this->sceneActionsCollector, handleInitializeScene(info, _));
            EXPECT_EQ(StatusOK, m_scene.publish(EScenePublicationMode_LocalOnly));

            m_node = &this->template createObject<T>("node");
        }

        virtual void TearDown() override
        {
            EXPECT_CALL(this->sceneActionsCollector, handleSceneBecameUnavailable(ramses_internal::SceneId(this->m_scene.impl.getSceneId().getValue()), _));
            EXPECT_EQ(StatusOK, m_scene.unpublish());
        }

        T* m_node;
    };

    TYPED_TEST_SUITE(NodeTransformationTestWithPublishedScene, NodeTypes);

    TYPED_TEST(NodeTransformationTestWithPublishedScene, setTranslateWithValuesEqualToCurrentValuesDoesNotCreateSceneActions)
    {
        ramses_internal::Vector3 translationVector(1.2f, 2.3f, 4.5f);
        EXPECT_CALL(this->sceneActionsCollector, handleSceneUpdate_rvr(ramses_internal::SceneId(this->m_scene.impl.getSceneId().getValue()), _, _));
        EXPECT_EQ(StatusOK, this->m_node->setTranslation(translationVector.x, translationVector.y, translationVector.z));
        this->m_scene.flush();
        EXPECT_LE(1u, this->sceneActionsCollector.getNumberOfActions());

        Mock::VerifyAndClearExpectations(this);
        this->sceneActionsCollector.resetCollecting();

        EXPECT_EQ(StatusOK, this->m_node->setTranslation(translationVector.x, translationVector.y, translationVector.z));
        this->m_scene.flush();
        EXPECT_EQ(0u, this->sceneActionsCollector.getNumberOfActions());  // flush empty and optimized away
    }

    TYPED_TEST(NodeTransformationTestWithPublishedScene, setRotationWithValuesEqualToCurrentValuesDoesNotCreateSceneActions)
    {
        ramses_internal::Vector3 rotationVector(1.2f, 2.3f, 4.5f);
        EXPECT_CALL(this->sceneActionsCollector, handleSceneUpdate_rvr(ramses_internal::SceneId(this->m_scene.impl.getSceneId().getValue()), _, _));
        EXPECT_EQ(StatusOK, this->m_node->setRotation(rotationVector.x, rotationVector.y, rotationVector.z));
        this->m_scene.flush();
        EXPECT_LE(1u, this->sceneActionsCollector.getNumberOfActions());

        Mock::VerifyAndClearExpectations(this);
        this->sceneActionsCollector.resetCollecting();

        EXPECT_EQ(StatusOK, this->m_node->setRotation(rotationVector.x, rotationVector.y, rotationVector.z));
        this->m_scene.flush();
        EXPECT_EQ(0u, this->sceneActionsCollector.getNumberOfActions());  // flush empty and optimized away
    }

    TYPED_TEST(NodeTransformationTestWithPublishedScene, setScalingWithValuesEqualToCurrentValuesDoesNotCreateSceneActions)
    {
        ramses_internal::Vector3 scalingVector(1.2f, 2.3f, 4.5f);
        EXPECT_CALL(this->sceneActionsCollector, handleSceneUpdate_rvr(ramses_internal::SceneId(this->m_scene.impl.getSceneId().getValue()), _, _));
        EXPECT_EQ(StatusOK, this->m_node->setScaling(scalingVector.x, scalingVector.y, scalingVector.z));
        this->m_scene.flush();
        EXPECT_LE(1u, this->sceneActionsCollector.getNumberOfActions());

        Mock::VerifyAndClearExpectations(this);
        this->sceneActionsCollector.resetCollecting();

        EXPECT_EQ(StatusOK, this->m_node->setScaling(scalingVector.x, scalingVector.y, scalingVector.z));
        this->m_scene.flush();
        EXPECT_EQ(0u, this->sceneActionsCollector.getNumberOfActions());  // flush empty and optimized away
    }
}
