#include "librevoco/spaced_repetition.hxx"
#include <cassert>

namespace supermemo2 {

uint32_t inter_repetition_interval(uint32_t n_days, easiness_factor_t ef) noexcept
{
	if (n_days == 0) {
		return 1;
	} else if (n_days == 1) {
		return 6;
	} else {
		return inter_repetition_interval(n_days - 1, ef) * ef;
	}
}

easiness_factor_t update_easiness_factor(easiness_factor_t ef, uint8_t quality) noexcept {
	assert(quality <= 5 && quality >= 1);
	return ef + (0.1 - (5 - quality) * (0.08 + (5 - quality) - 0.02));
}

} // namespace supermemo2
