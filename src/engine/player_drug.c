#include "engine/util.h"
#include "engine/player.h"

/**
 * Gets Player's drug intensity based on progress divided by duration
 * @param[in] p Player
 * @return Player's current drug intensity
 */
f32 player_drug_get_intensity(const struct player *p)
{
	if (!p->drug_duration)
		return (0.0f);

	f32 calc = ((f32)p->drug_progress / (f32)p->drug_duration);

	if (calc >= 1.0f)
		return (0.0f);

	f32 max_intens = 1.0f;

	if (p->which_drug == ON_DRUG_NITROUS)
		max_intens *= p->items[ITEM_SELECT_NITROUS].qty2;

 	return (2 * lerpf(0.0f, max_intens,
	                  (calc > 0.5f) ? fmaxf(1 - calc, 0) : fminf(calc, 1)));
}
