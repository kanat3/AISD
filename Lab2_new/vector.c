#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

Deque* create_dq () {

	Deque* deque = (Deque*)malloc(sizeof(Deque));
	deque->front = -1;
	deque->rear = 0;
	deque->clearTime = 0;
	deque->size = ID_MAX;

	return deque;
}

int push_front (Deque* deque, Patient patient) {
	// если очередь уже полная
	if (is_full(deque)) {
		return 0;
	}
	// если очередь пустая, то при вставке присвоим индекс 0 первому элементу
	if (deque->front == -1) {
		deque->front = 0;
		deque->rear = 0;
	}
	// если очередь непустая и в ней 1 элемент
	else if (deque->front == 0) {
		deque->front = deque->size - 1;
	}
	else {
		// если непустая и больше 1-го элемента уменьшаем индекс первого элемента на 1
		--deque->front;
	}
	// вставляем элемент в начало очереди
	deque->arr[deque->front] = patient;
	return 1;
}

int push_rear (Deque* deque, Patient patient) {
	// если очередь уже полная
	if (is_full(deque)) {
		return 0;
	}
	// если очередь пустая, то при вставке присвоим индекс 0 первому элементу
	if (deque->front == -1) {
		deque->front = 0;
		deque->rear = 0;
	}
	// если очередь непустая и хвост последняя позиция в очереди
	else if (deque->rear == deque->size - 1) {
		deque->rear = 0;
	}
	else {
		++deque->rear;
	}
	// вставляем элемент в конец очереди
	deque->arr[deque->rear] = patient;
	return 1;
}

int pop_front (Deque* deque) {

	if (is_empty(deque)) {
		return 0;
	}

	if (deque->front == deque->rear) {
		deque->front = -1;
		deque->rear = -1;
	} else

	if (deque->front == deque->size - 1) {
		deque->front = 0;
	} else {
		deque->front = deque->front + 1;
	}
	return 1;

}

int pop_rear (Deque* deque) {
	if (is_empty(deque)) {
		return 0;
	}
	// Если в очереди только один элемент
	if (deque->front == deque->rear) {
		deque->front = -1;
		deque->rear = -1;
	} else if (deque->rear == 0) {
		deque->rear = deque->size - 1;
	} else {
		deque->rear = deque->rear - 1;
	}
	return 1;
}

Patient get_front (Deque* deque) {
	if (is_empty(deque)) {
		Patient patient;
		patient.appear = -1;
		patient.duration = 0;
		patient.tm = 0;
		return patient;
	}
	return deque->arr[deque->front];
}

Patient get_rear (Deque* deque) {
	if (is_empty(deque)) {
		Patient patient;
		patient.appear = -1;
		patient.duration = 0;
		patient.tm = 0;
		return patient;
	}
	return deque->arr[deque->rear];
}

void show_patient (Patient* patient) {
	printf("%s/%d/%d  | waiting time: %d\n", patient->id, patient->appear, patient->duration, patient->tm);
}

int is_full (Deque* deque) {
	return ((deque->front == 0 && deque->rear == sizeof(deque->arr) - 1) || deque->front == deque->rear + 1);
}

int is_empty (Deque* deque) {
	return (deque->front == -1);
}

void show_dq (Deque* deque) {
	int i;
	if (deque->front > deque->rear) {
		for (i = deque->front; i < deque->size; ++i) {
			show_patient(&deque->arr[i]);
		}
		for (i = 0; i <= deque->rear; ++i) {
			show_patient(&deque->arr[i]);
		}
	} else {
		for (i = deque->front; i <= deque->rear; ++i) {
			show_patient(&deque->arr[i]);
		}
	}
}

void time_up (Deque* deque, Patient* patient, int t) {
	int i, counter = 0;
	if (deque->front > deque->rear) {
		for (i = deque->front; i < deque->size; ++i) {
			if (counter > 1) {
				deque->arr[i].tm += patient->duration;
			}
			++counter;
		}
		for (i = 0; i <= deque->rear; ++i) {
			if (counter > 1) {
				deque->arr[i].tm += patient->duration;
			}
			++counter;
		}
	} else {
		for (i = deque->front; i <= deque->rear; ++i) {
			if (counter > 1) {
				deque->arr[i].tm += patient->duration;
			}
			++counter;
		}
	}
}

void time_down (Deque* deque, int t) {

	int i, koef = 0;
	if (deque->arr[deque->front].appear == 1 && !deque->clearTime)
		koef = 1;
	deque->arr[deque->front].tm = deque->arr[deque->front].duration - t + koef + deque->clearTime;

	if (deque->front > deque->rear) {
		for (i = deque->front + 1; i < deque->size; ++i) {
			deque->arr[i].tm = deque->arr[i - 1].tm + deque->arr[i].duration;
		}
		deque->arr[0].tm = deque->arr[deque->size - 1].tm + deque->arr[0].duration;

		for (i = 1; i <= deque->rear; ++i) {
			deque->arr[i].tm = deque->arr[i - 1].tm + deque->arr[i].duration;
		}
	} else {
		for (i = deque->front + 1; i <= deque->rear; ++i) {
			deque->arr[i].tm = deque->arr[i - 1].tm + deque->arr[i].duration;
		}
	}

}
