/* RandomEvent.h
Copyright (c) 2021 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RANDOMEVENT_H_
#define RANDOMEVENT_H_



// A class representing an event that triggers randomly
// in a given internal, for example fleets or hazards.
template <typename T>
class RandomEvent {
public:
	static const int DEFAULT_PERIOD = 200;


	constexpr RandomEvent(const T *event, int period) noexcept;

	constexpr const T *Get() const noexcept;
	constexpr int Period() const noexcept;
	int &Period() noexcept;

private:
	const T *event;
	int period;
};



template <typename T>
constexpr RandomEvent<T>::RandomEvent(const T *event, int period) noexcept
	: event(event), period(period > 0 ? period : DEFAULT_PERIOD)
{
}

template <typename T>
constexpr const T *RandomEvent<T>::Get() const noexcept
{
	return event;
}

template <typename T>
constexpr int RandomEvent<T>::Period() const noexcept
{
	return period;
}

template <typename T>
int &RandomEvent<T>::Period() noexcept
{
	return period;
}



#endif
