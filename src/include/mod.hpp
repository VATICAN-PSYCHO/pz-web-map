#ifndef MOD_HPP
#define MOD_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;

enum class ModType { MAP, TEXTURE, MIXED };

class Mod {
  public:
	Mod(const std::string &id, const std::uint64_t &steamId,
		const std::string &name, const std::string &pack,
		const std::string &path);
	~Mod();

	void addDependency(std::shared_ptr<Mod> mod);
	void addDependant(std::shared_ptr<Mod> mod);

	vector<std::shared_ptr<Mod>> getDependencies() const;
	vector<std::shared_ptr<Mod>> getDependants() const;
	vector<string> getRawDependencies() const;
	void setRawDependencies(const vector<string> rawDependencies);
	void setType(ModType type);
	ModType getType() const;

  private:
	string id;
	uint64_t steamId;
	string name;
	string pack;
	ModType type;
	string path;
	vector<string> rawDependencies;
	vector<std::shared_ptr<Mod>> dependencies;
	vector<std::shared_ptr<Mod>> dependants;
};

#endif