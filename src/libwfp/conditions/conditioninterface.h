#pragma once

#include "comparison.h"
#include "ifiltercondition.h"
#include "libcommon/buffer.h"
#include <windows.h>
#include <memory>

namespace wfp::conditions {

class ConditionInterface : public IFilterCondition
{
	struct ctor_tag { explicit ctor_tag() = default; };

	enum class IdentifierType
	{
		Index,
		Luid,
		Alias,
		Name
	};

public:

	// Public but non-invokable
	ConditionInterface(uint32_t interfaceIndex, const IStrictComparison &comparison, ctor_tag);
	ConditionInterface(uint64_t interfaceLuid, const IStrictComparison &comparison, ctor_tag);
	ConditionInterface(const std::wstring &interfaceIdentifier, IdentifierType type, const IStrictComparison &comparison, ctor_tag);

	static std::unique_ptr<ConditionInterface> Index(uint32_t interfaceIndex, const IStrictComparison &comparison = CompareEq());
	static std::unique_ptr<ConditionInterface> Luid(uint64_t interfaceLuid, const IStrictComparison &comparison = CompareEq());
	static std::unique_ptr<ConditionInterface> Alias(const std::wstring &interfaceAlias, const IStrictComparison &comparison = CompareEq());
	static std::unique_ptr<ConditionInterface> Name(const std::wstring &interfaceName, const IStrictComparison &comparison = CompareEq());

	std::wstring toString() const override;
	const GUID &identifier() const override;
	const FWPM_FILTER_CONDITION0 &condition() const override;

private:

	UINT32 m_interfaceIndex;
	UINT64 m_interfaceLuid;
	std::wstring m_interfaceIdentifier;

	IdentifierType m_identifierType;
	ComparisonSpecification m_comparison;

	common::Buffer m_assembled;
};

}
