// Calculate your age by your date of birth.

function getAge(birthYear: number, birthMonth: number, birthDay: number): number {
  const today = new Date();
  const currentYear = today.getFullYear();
  const currentMonth = today.getMonth() + 1; // 0-indexed -> + 1!
  const currentDay = today.getDate();

  let age = currentYear - birthYear;

  // Birthday hasn't occurred yet this year:
  if (currentMonth < birthMonth || (currentMonth === birthMonth && currentDay < birthDay))
  {
    age--;
  }

  return age;
}

const age = getAge(1982, 1, 21);
