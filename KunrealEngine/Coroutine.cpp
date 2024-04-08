#include "Coroutine.h"
#include "TimeManager.h"

namespace KunrealEngine
{
	std::vector<KunrealEngine::Coroutine::Coroutine_type*> KunrealEngine::Coroutine::_coroutines;
	std::map<int, std::function<KunrealEngine::Coroutine::Coroutine_type()>> KunrealEngine::Coroutine::_AddedCoroutines; // 코루틴의 주소를 저장하는 집합
	int KunrealEngine::Coroutine::idexKey;

	Coroutine::Coroutine()
	{

	};

	Coroutine::~Coroutine()
	{

	};

	/////////////////////////////////////////
	/////////////DurationManager/////////////
	/////////////////////////////////////////

	Coroutine::DurationManager& Coroutine::DurationManager::getInstance()
	{
		static DurationManager instance;
		return instance;
	}

	float Coroutine::DurationManager::getDuration() const
	{
		return duration;
	}

	void Coroutine::DurationManager::setDuration(float newDuration)
	{
		duration = newDuration;
	}

	/////////////////////////////////
	/////////////Coroutine///////////
	/////////////////////////////////

	Coroutine::Coroutine_type::Coroutine_type()
	{

	}

	Coroutine::Coroutine_type::Coroutine_type(std::coroutine_handle<promise_type> handle) : coro_handle(handle)
	{

	}

	Coroutine::Coroutine_type::~Coroutine_type()
	{
		coro_handle.destroy();
	}

	////////////////////////////////////
	/////////////promise_type///////////
	////////////////////////////////////

	Coroutine::Coroutine_type Coroutine::Coroutine_type::promise_type::get_return_object()
	{
		return Coroutine_type(std::coroutine_handle<promise_type>::from_promise(*this));
	}

	std::suspend_never Coroutine::Coroutine_type::promise_type::initial_suspend() noexcept
	{
		return {};
	}

	std::suspend_always Coroutine::Coroutine_type::promise_type::final_suspend() noexcept
	{
		return {};
	}

	void Coroutine::Coroutine_type::promise_type::return_void() noexcept
	{

	}

	void Coroutine::Coroutine_type::promise_type::unhandled_exception() noexcept
	{

	}

	bool Coroutine::Coroutine_type::promise_type::await_ready() noexcept
	{
		bool isReady;
		timer += TimeManager::GetInstance().GetDeltaTime();

		if (timer > DurationManager::getInstance().getDuration())
		{
			timer = 0;
			isReady = true;
		}

		else
		{
			isReady = false;
		}

		return isReady;
	}

	//////////////////////////////////////
	/////////////WaitForSeconds///////////
	//////////////////////////////////////

	// WaitForSeconds 구현
	Coroutine::Coroutine_type::WaitForSeconds::WaitForSeconds(float seconds)
	{
		DurationManager::getInstance().setDuration(seconds);
	}

	bool Coroutine::Coroutine_type::WaitForSeconds::await_ready()
	{
		return false;
	}

	void Coroutine::Coroutine_type::WaitForSeconds::await_suspend(std::coroutine_handle<> handle)
	{

	}

	void Coroutine::Coroutine_type::WaitForSeconds::await_resume() const noexcept {}

	//////////////////////////////////////
	/////////////StartCoroutine///////////
	//////////////////////////////////////

	void Coroutine::StartCoroutine(std::function<Coroutine_type()> coro)
	{
		// 이미 추가된 코루틴인지 확인
		for (auto& coron : _AddedCoroutines)
		{
			if (coron.second.target_type() == coro.target_type())
			{
				return;
			}
		}
		
		Coroutine_type* coroutineInstance = new Coroutine_type(coro());
		_coroutines.emplace_back(coroutineInstance);
		idexKey++;
		coroutineInstance->mapKey = idexKey;

		_AddedCoroutines.insert(std::make_pair(idexKey, coro));
	}

	////////////////////////////////////////
	/////////////UpdateCoroutines///////////
	////////////////////////////////////////

	void Coroutine::UpdateCoroutines()
	{
		for (auto& coroutine : _coroutines)
		{
			if (!coroutine->coro_handle.done() && coroutine->coro_handle.promise().await_ready())
			{
				// 코루틴이 완료되지 않고, 지정된 시간이 경과하지 않은 경우 resume하지 않음
				coroutine->coro_handle.resume();
			}

			else if (coroutine->coro_handle.done())
			{
				_AddedCoroutines.erase(coroutine->mapKey);

				auto iter = std::find(_coroutines.begin(), _coroutines.end(), coroutine);
				delete coroutine;
				coroutine = nullptr;
				_coroutines.erase(iter); // 벡터에서 제거
			}
		}
	}
}