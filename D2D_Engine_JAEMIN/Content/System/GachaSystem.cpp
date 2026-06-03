#include "GachaSystem.h"

void GachaSystem::LoadDefaultData()
{
    // 확률의 합은 100이 되도록 구성합니다.
    // 나중에는 이 테이블을 CSV/JSON으로 옮기면 코드 수정 없이 밸런스를 바꿀 수 있습니다.
    m_items =
    {
        { 1, L"평범한 돌", ItemGrade::Common, 35.0f, 1 },
        { 2, L"작은 조약돌", ItemGrade::Common, 25.0f, 1 },
        { 3, L"반짝이는 돌", ItemGrade::Rare, 15.0f, 3 },
        { 4, L"단단한 돌", ItemGrade::Rare, 15.0f, 3 },
        { 5, L"황금빛 돌", ItemGrade::Epic, 5.0f, 10 },
        { 6, L"고대의 돌", ItemGrade::Epic, 4.0f, 10 },
        { 7, L"성공의 돌", ItemGrade::Legendary, 1.0f, 100 }
    };
}

GachaItem GachaSystem::PickItem()
{
    if (m_items.empty())
    {
        return {};
    }

    std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
    const float roll = distribution(m_randomEngine);
    float cumulativeProbability = 0.0f;

    for (const GachaItem& item : m_items)
    {
        cumulativeProbability += item.probability;
        if (roll <= cumulativeProbability)
        {
            return item;
        }
    }

    // 부동소수점 오차로 100을 아주 살짝 넘는 경우를 대비한 안전 장치입니다.
    return m_items.back();
}

const std::vector<GachaItem>& GachaSystem::GetItems() const
{
    return m_items;
}
