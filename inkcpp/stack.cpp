#include "stack.h"

namespace ink
{
	namespace runtime
	{
		namespace internal
		{
			basic_stack::basic_stack(entry* data, size_t size)
				: _stack(data), _size(size), _pos(0), _save(~0), _jump(~0)
			{

			}

			void basic_stack::set(hash_t name, const value& val)
			{
				// If we have a save point, always add no matter what
				if (_save != ~0)
				{
					add(name, val);
					return;
				}

				// Either set an existing variable or add it to the stack
				value* existing = const_cast<value*>(get(name));
				if (existing == nullptr)
					add(name, val);
				else
					*existing = val;
			}

			const value* basic_stack::get(hash_t name) const
			{
				if (_pos == 0)
					return nullptr;

				// Move backwards and find the variable
				size_t i = _pos;
				do
				{
					i--;

					if (_stack[i].name == name)
						return &_stack[i].data;

					// We hit the top of this stack frame. Not found!
					if (_stack[i].name == InvalidHash)
						break;

					// Jump over saved data
					if (i == _save)
						i = _jump;
				} while (i > 0);

				return nullptr;
			}

			void basic_stack::push_frame(offset_t return_to, frame_type type)
			{
				// Add to top of stack
				add(InvalidHash, value(return_to, type == frame_type::tunnel ? data_type::tunnel_frame : data_type::function_frame));
			}

			offset_t basic_stack::pop_frame(frame_type* type)
			{
				inkAssert(_pos > 0, "Can not pop frame from empty callstack");

				// Advance up the callstack until we find the frame record
				_pos--;
				while (_stack[_pos].name != InvalidHash && _pos > 0)
				{
					// Jump over saved data
					if (_pos == _save)
						_pos = _jump;

					_pos--;
				}

				inkAssert(_stack[_pos].name == InvalidHash, "Attempting to pop_frame when no frames exist! Stack reset");

				// Store frame type
				if (type != nullptr)
					*type = (_stack[_pos].data.data_type() == data_type::tunnel_frame) ? frame_type::tunnel : frame_type::function;

				// Return the offset stored in the frame record
				return _stack[_pos].data.as_divert();
			}

			bool basic_stack::has_frame() const
			{
				// Empty case
				if (_pos == 0)
					return false;

				size_t iter = _pos - 1;
				while (_stack[iter].name != InvalidHash && iter > 0)
				{
					// Jump over saved data
					if (iter == _save)
						iter = _jump;

					iter--;
				}
				return _stack[iter].name == InvalidHash;
			}

			void basic_stack::clear()
			{
				_save = _jump = ~0;
				_pos = 0;
			}

			void basic_stack::mark_strings(string_table& strings) const
			{
				// no matter if we're saved or not, we consider all strings
				int len = (_save == ~0 || _pos > _save) ? _pos : _save;

				// iterate and mark
				for (int i = 0; i < len; i++)
					_stack[i].data.mark_strings(strings);
			}

			void basic_stack::save()
			{
				inkAssert(_save == ~0, "Can not save stack twice! restore() or forget() first");

				// Save current stack position
				_save = _jump = _pos;
			}

			void basic_stack::restore()
			{
				inkAssert(_save != ~0, "Can not restore() when there is no save!");

				// Move position back to saved position
				_pos = _save;
				_save = _jump = ~0;
			}

			void basic_stack::forget()
			{
				inkAssert(_save != ~0, "Can not forget when the stack has never been saved!");

				// If we have moven to a point earlier than the save point but we have a jump point
				if (_pos < _save && _pos > _jump)
				{
					// Everything between the jump point and the save point needs to be nullified
					for (size_t i = _jump; i < _save; i++)
						_stack[i].name = ~0;
				}

				// Just reset save position
				_save = ~0;
			}

			void basic_stack::add(hash_t name, const value& val)
			{
				// Don't destroy saved data
				if (_pos < _save && _save != ~0)
				{
					// Move to next safe spot after saved data and save where we came from
					_jump = _pos;
					_pos = _save;
				}

				inkAssert(_pos < _size, "Stack overflow!");

				// Push onto the top of the stack
				_stack[_pos].name = name;
				_stack[_pos].data = val;
				_pos++;
			}

			basic_eval_stack::basic_eval_stack(value* data, size_t size)
				: _stack(data), _size(size), _pos(0), _save(~0), _jump(~0)
			{

			}

			void basic_eval_stack::push(const value& val)
			{
				// Don't destroy saved data. Jump over it
				if (_pos < _save && _save != ~0)
				{
					_jump = _pos;
					_pos = _save;
				}

				inkAssert(_pos < _size, "Stack overflow!");
				_stack[_pos++] = val;
			}

			value basic_eval_stack::pop()
			{
				inkAssert(_pos > 0, "Nothing left to pop!");

				// Jump over save data
				if (_pos == _save)
					_pos = _jump;

				// Move over none data
				while (_stack[_pos].is_none())
					_pos--;

				// Decrement and return
				_pos--;
				return _stack[_pos];
			}

			const value& basic_eval_stack::top() const
			{
				inkAssert(_pos > 0, "Stack is empty! No top()");

				return _stack[_pos - 1];
			}

			bool basic_eval_stack::is_empty() const
			{
				return _pos == 0 || _stack[_pos - 1].data_type() == data_type::thread_start;
			}

			void basic_eval_stack::clear()
			{
				_pos = 0;
				_jump = _save = ~0;
			}

			void basic_eval_stack::mark_strings(string_table& strings) const
			{
				// no matter if we're saved or not, we consider all strings
				int len = (_save == ~0 || _pos > _save) ? _pos : _save;

				for (int i = 0; i < len; i++)
					_stack[i].mark_strings(strings);
			}

			void basic_eval_stack::save()
			{
				inkAssert(_save == ~0, "Can not save stack twice! restore() or forget() first");

				// Save current stack position
				_save = _jump = _pos;
			}

			void basic_eval_stack::restore()
			{
				inkAssert(_save != ~0, "Can not restore() when there is no save!");

				// Move position back to saved position
				_pos = _save;
				_save = _jump = ~0;
			}

			void basic_eval_stack::forget()
			{
				inkAssert(_save != ~0, "Can not forget when the stack has never been saved!");

				// If we have moven to a point earlier than the save point but we have a jump point
				if (_pos < _save && _pos > _jump)
				{
					// Everything between the jump point and the save point needs to be nullified
					data x; x.set_none();
					value none = value(x);
					for (size_t i = _jump; i < _save; i++)
						_stack[i] = none;
				}

				// Just reset save position
				_save = ~0;
			}

			const value thread_start = value(0, data_type::thread_start);

			size_t basic_eval_stack::find_thread_start() const
			{
				if (_pos == 0)
					return 0;

				for (size_t i = _pos - 1; i > 0; i--)
				{
					// Return position right after thread start marker
					if (_stack[i].data_type() == data_type::thread_start)
						return i;

					// Move to stored position
					if (_stack[i].data_type() == data_type::thread_callback)
						i = _stack[i].as_divert();
				}

				return 0;
			}

			void basic_eval_stack::thread_fork(size_t i)
			{
				// Get stack entry at position
				const value& entry = _stack[i];
				data_type type = entry.data_type();

				// If we're at a thread start, we're ready. Return and let the forking begin
				if (type == data_type::thread_start)
					return;

				// If we hit a thread callback, the stack resumes earlier
				if (type == data_type::thread_callback)
				{
					// Move to where it resumes
					thread_fork(entry.as_divert() - 1);
					return;
				}

				// If we're not done yet
				else if(i > 0)
				{
					// Recurse
					thread_fork(i - 1);
				}

				// Don't bother copying nullified entries
				if (entry.data_type() == data_type::none)
					return;
				
				// Duplicate
				push(entry);
			}

			void basic_eval_stack::thread_copy(size_t i, size_t& dst)
			{
				// Get stack entry at position
				const value& entry = _stack[i];
				data_type type = entry.data_type();

				// If we're at a thread start, we're ready. Return and let the forking begin
				if (type == data_type::thread_start)
					return;

				// If we hit a thread callback, the stack resumes earlier
				if (type == data_type::thread_callback)
				{
					// Move to where it resumes
					thread_copy(entry.as_divert() - 1, dst);
					return;
				}

				// If we're not done yet
				else if (i > 0)
				{
					// Recurse
					thread_copy(i - 1, dst);
				}

				// Don't bother copying nullified entries
				if (entry.data_type() == data_type::none)
					return;

				// Copy
				_stack[dst++] = _stack[i];
			}

			void basic_eval_stack::thread_fork()
			{
				inkAssert(_save == ~0, "Threading while saved is untested!");

				// Here we go. Push a new thread
				push(thread_start);

				// Fork existing thread
				if(_pos > 2)
					thread_fork(_pos - 2);
			}

			void basic_eval_stack::thread_resume()
			{
				inkAssert(_save == ~0, "Threading while saved is untested!");

				// Move back to where the thread began
				size_t start = find_thread_start();
				_pos = start;

				// TODO: Modify push/pos to respect thread shit
				// Store largest index? That needs save restore?
			}

			void basic_eval_stack::thread_collapse(size_t saved)
			{
				inkAssert(_save == ~0, "Threading while saved is untested!");

				// Special: clear
				if (saved == 0)
				{
					_pos = 0;
					return;
				}

				// Okay. Here we go. 
				size_t end = 0;
				thread_copy(saved - 1, end);

				// We're not at the end
				_pos = end;

				// TODO: Max position as mentioned above?
			}

			size_t basic_eval_stack::thread_save() const
			{
				inkAssert(_save == ~0, "Threading while saved is untested!");

				return _pos;
			}

		}
	}
}